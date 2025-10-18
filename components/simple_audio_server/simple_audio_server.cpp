#include "simple_audio_server.h"
#ifdef USE_WEBSERVER_OTA

#include "esphome/core/application.h"
#include "esphome/core/log.h"

#ifdef USE_CAPTIVE_PORTAL
#include "esphome/components/captive_portal/captive_portal.h"
#endif

using PlatformString = std::string;

namespace esphome {
namespace web_server {

static const char *const TAG = "simple_audio_server";

class SASRequestHandler : public AsyncWebHandler, public Component {
 public:
  SASRequestHandler(SimpleAudioServerComponent *parent) : parent_(parent) {}
  void handleRequest(AsyncWebServerRequest *request) override;

  // Handle only /sas requests
  bool canHandle(AsyncWebServerRequest *request) const override {
    bool is_sas_request = request->url().rfind("/sas", 0) == 0 && request->method() == HTTP_GET;
    return is_sas_request;
  }

  bool isRequestHandlerTrivial() const override { return false; }

 protected:
  SimpleAudioServerComponent *parent_;
};

void SASRequestHandler::handleRequest(AsyncWebServerRequest *request) {
  AsyncWebServerResponse *serverResponse;

  if (request->url() == "/sas") {
    std::string response = "SOAS Audio File Server (Offline Mode)\n";
    response += "====================================\n";
    response += "Alarm file: " + parent_->alarm_filename_ + "\n";
    response += "Status: " + std::string((parent_->audio_data_ && parent_->audio_size_ > 0) ? "Ready" : "No audio data") + "\n";
    response += "Stream URL: http://127.0.0.1:" + to_string(parent_->port_) + "/" + parent_->alarm_filename_ + "\n";
    response += "\nFor SOAS alarm configuration:\n";
    response += "Add this URL to your alarm_stream_url options in YAML\n";

    if (parent_->audio_data_ && parent_->audio_size_ > 0) {
      response += "\nEmbedded Audio Info:\n";
      response += "Size: " + to_string(parent_->audio_size_) + " bytes\n";
      response += "Looping: " + std::string(parent_->audio_size_ < 500000 ? "YES (infinite)" : "NO (single play)") + "\n";
    }

    response += "\nRequest details:\n";
    response += "Host: " + request->host() + "\n";
    //response += "Client IP: " + request->client()->remoteIP().toString() + "\n";
    response += "\nMODE: FULLY OFFLINE - works without any network!\n";

    serverResponse = request->beginResponse(200, "text/plain", response);
    serverResponse->addHeader("Connection", "close");
    request->send(serverResponse);
  }
  else {
    ESP_LOGI(TAG, "=== AUDIO REQUEST RECEIVED ===");
    //ESP_LOGI(TAG, "Client: %s", request->client()->remoteIP().toString().c_str());
    ESP_LOGI(TAG, "URL: %s", request->url().c_str());
    //ESP_LOGI(TAG, "Method: %s", request->methodToString());

    if (parent_->alarm_filename_.empty()) {
      ESP_LOGW(TAG, "No alarm file configured");
      serverResponse = request->beginResponse(404, "text/plain", "No alarm file configured");
      request->send(serverResponse);
      return;
    }

    if (parent_->audio_data_ == nullptr || parent_->audio_size_ == 0) {
      ESP_LOGW(TAG, "No audio data available for: %s", parent_->alarm_filename_.c_str());
      ESP_LOGW(TAG, "Audio data pointer: %p, size: %d", parent_->audio_data_, parent_->audio_size_);

      // Send a test response to verify the server is working
      std::string test_response = "Audio Server Test Response\n";
      test_response += "File requested: " + parent_->alarm_filename_ + "\n";
      test_response += "Audio data: " + std::string(parent_->audio_data_ ? "Available" : "NULL") + "\n";
      test_response += "Audio size: " + to_string(parent_->audio_size_) + " bytes\n";
      test_response += "Check compilation logs for embedding status\n";
      test_response += "Mode: Offline capable\n";

      serverResponse = request->beginResponse(404, "text/plain", test_response);
      request->send(serverResponse);
      return;
    }

    // Determine content type based on file extension
    std::string content_type = "audio/mpeg";
    std::string lower_filename = str_lower_case(parent_->alarm_filename_.c_str());

    if (lower_filename.ends_with(".mp3")) content_type = "audio/mpeg";
    if (lower_filename.ends_with(".wav")) content_type = "audio/wav";
    if (lower_filename.ends_with(".flac")) content_type = "audio/flac";
    if (lower_filename.ends_with(".aac")) content_type = "audio/aac";
    if (lower_filename.ends_with(".m4a")) content_type = "audio/mp4";
    if (lower_filename.ends_with(".ogg")) content_type = "audio/ogg";

    ESP_LOGI(TAG, "Serving looped audio stream: %s (%d bytes, %s)",
            parent_->alarm_filename_.c_str(), parent_->audio_size_, content_type.c_str());

    // Send the audio data
    AsyncWebServerResponse *response = request->beginResponse(200, content_type.c_str(), parent_->audio_data_, parent_->audio_size_);

    response->addHeader("Accept-Ranges", "bytes");
    response->addHeader("Cache-Control", "no-cache");

    // For single-play files, set content length
    std::string audio_size = std::to_string(parent_->audio_size_);
    response->addHeader("Content-Length", audio_size.c_str());
    ESP_LOGI(TAG, "Serving stream (%s bytes)", audio_size.c_str());
    request->send(response);
  }
}

void SimpleAudioServerComponent::set_alarm_file(const std::string& filename) {
  alarm_filename_ = filename;
  ESP_LOGI(TAG, "Alarm file set to: %s", filename.c_str());
}

void SimpleAudioServerComponent::set_audio_data(const uint8_t* data, size_t size) {
  audio_data_ = data;
  audio_size_ = size;
  ESP_LOGI(TAG, "Audio data set: %d bytes", size);
}

void SimpleAudioServerComponent::setup() {
  // Get the global web server base instance and register our handler
  auto *base = web_server_base::global_web_server_base;
  if (base == nullptr) {
    ESP_LOGE(TAG, "WebServerBase not found");
    this->mark_failed();
    return;
  }

  // AsyncWebServer takes ownership of the handler and will delete it when the server is destroyed
  base->add_handler(new SASRequestHandler(this));  // NOLINT
}

void SimpleAudioServerComponent::dump_config() { ESP_LOGCONFIG(TAG, "Simple audio server"); }

}  // namespace web_server
}  // namespace esphome

#endif  // USE_WEBSERVER_OTA