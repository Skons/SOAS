#include "simple_audio_server.h"

namespace esphome {
namespace simple_audio_server {

SimpleAudioServerComponent::SimpleAudioServerComponent() {}

void SimpleAudioServerComponent::setup() {
  ESP_LOGCONFIG(TAG, "Setting up Simple Audio Server (Embedded mode)...");
  
  // Check if audio data is available
  if (audio_data_ != nullptr && audio_size_ > 0) {
    ESP_LOGI(TAG, "Embedded audio file ready: %s (%d bytes)", 
             alarm_filename_.c_str(), audio_size_);
  } else {
    ESP_LOGW(TAG, "No audio data embedded - server will respond with 404");
    ESP_LOGW(TAG, "Make sure %s is in your ESPHome project directory", alarm_filename_.c_str());
  }
  
  // Initialize WiFi interface for localhost networking (but don't connect)
  WiFi.mode(WIFI_STA);
  ESP_LOGI(TAG, "WiFi interface initialized for localhost networking (offline mode)");
  
  ESP_LOGI(TAG, "Starting server setup in 3 seconds...");
  
  this->set_timeout(3000, [this]() {
    ESP_LOGI(TAG, "Timeout callback triggered - starting offline-capable server");
    this->start_server();
  });
  
  ESP_LOGCONFIG(TAG, "Simple Audio Server setup complete - FULLY OFFLINE CAPABLE");
}

void SimpleAudioServerComponent::start_server() {
  ESP_LOGI(TAG, "start_server() called - starting localhost server");
  
  // Server works completely offline - no WiFi connection required
  ESP_LOGI(TAG, "Starting server in offline-capable mode");
  ESP_LOGI(TAG, "This will work even without WiFi for localhost (127.0.0.1) requests");
  
  ESP_LOGI(TAG, "Creating AsyncWebServer on port %d...", port_);
  ESP_LOGI(TAG, "Server will bind to 0.0.0.0:%d (accepts localhost requests)", port_);
  ESP_LOGI(TAG, "Localhost stream works WITHOUT internet connectivity");
  
  server_ = std::make_unique<AsyncWebServer>(port_);
  
  if (!server_) {
    ESP_LOGE(TAG, "Failed to create AsyncWebServer");
    return;
  }
  
  ESP_LOGI(TAG, "Adding server endpoints...");
  
  // Root endpoint with SOAS-style info
  server_->on("/", HTTP_GET, [this](AsyncWebServerRequest *request) {
    ESP_LOGI(TAG, "ROOT REQUEST from %s to %s", 
             request->client()->remoteIP().toString().c_str(),
             request->host().c_str());
    
    String response = "SOAS Audio File Server (Offline Mode)\n";
    response += "====================================\n";
    response += "Alarm file: " + String(alarm_filename_.c_str()) + "\n";
    response += "Status: " + String((audio_data_ && audio_size_ > 0) ? "Ready" : "No audio data") + "\n";
    response += "Stream URL: http://127.0.0.1:" + String(port_) + "/" + String(alarm_filename_.c_str()) + "\n";
    response += "\nFor SOAS alarm configuration:\n";
    response += "Add this URL to your alarm_stream_url options in YAML\n";
    
    if (audio_data_ && audio_size_ > 0) {
      response += "\nEmbedded Audio Info:\n";
      response += "Size: " + String(audio_size_) + " bytes\n";
      response += "Looping: " + String(audio_size_ < 500000 ? "YES (infinite)" : "NO (single play)") + "\n";
    }
    
    response += "\nRequest details:\n";
    response += "Host: " + request->host() + "\n";
    response += "Client IP: " + request->client()->remoteIP().toString() + "\n";
    response += "\nMODE: FULLY OFFLINE - works without any network!\n";
    
    request->send(200, "text/plain", response);
  });
  
  // Direct alarm file endpoint
  if (!alarm_filename_.empty()) {
    String endpoint = "/" + String(alarm_filename_.c_str());
    ESP_LOGI(TAG, "Setting up endpoint: %s", endpoint.c_str());
    server_->on(endpoint.c_str(), HTTP_GET, [this](AsyncWebServerRequest *request) {
      ESP_LOGI(TAG, "=== ALARM AUDIO REQUEST from %s to %s ===", 
               request->client()->remoteIP().toString().c_str(),
               request->host().c_str());
      this->handle_audio_stream_(request);
    });
  }
  
  // Generic audio file endpoint (fallback)
  server_->onNotFound([this](AsyncWebServerRequest *request) {
    String uri = request->url();
    ESP_LOGI(TAG, "NOT FOUND REQUEST: %s from %s to %s", 
             uri.c_str(),
             request->client()->remoteIP().toString().c_str(),
             request->host().c_str());
    
    // Check if it's requesting our alarm file
    if (uri == ("/" + String(alarm_filename_.c_str()))) {
      ESP_LOGI(TAG, "Redirecting to audio handler");
      this->handle_audio_stream_(request);
    } else {
      ESP_LOGW(TAG, "Unknown request: %s", uri.c_str());
      String error_response = "File not found\n";
      error_response += "Available: /" + String(alarm_filename_.c_str()) + "\n";
      error_response += "Embedded data: " + String((audio_data_ && audio_size_ > 0) ? "Available" : "None");
      request->send(404, "text/plain", error_response);
    }
  });
  
  server_->begin();
  
  ESP_LOGI(TAG, "=== SOAS AUDIO SERVER STARTED SUCCESSFULLY ===");
  ESP_LOGI(TAG, "Server running on port %d", port_);
  ESP_LOGI(TAG, "SOAS stream URL: http://127.0.0.1:%d/%s", port_, alarm_filename_.c_str());
  ESP_LOGI(TAG, "Mode: FULLY OFFLINE CAPABLE - no network required");
  ESP_LOGI(TAG, "WiFi status: %s (not required for localhost streaming)", 
           WiFi.status() == WL_CONNECTED ? "Connected" : "Offline");
}

void SimpleAudioServerComponent::handle_audio_stream_(AsyncWebServerRequest *request) {
  ESP_LOGI(TAG, "=== AUDIO REQUEST RECEIVED ===");
  ESP_LOGI(TAG, "Client: %s", request->client()->remoteIP().toString().c_str());
  ESP_LOGI(TAG, "URL: %s", request->url().c_str());
  ESP_LOGI(TAG, "Method: %s", request->methodToString());
  
  if (alarm_filename_.empty()) {
    ESP_LOGW(TAG, "No alarm file configured");
    request->send(404, "text/plain", "No alarm file configured");
    return;
  }
  
  if (audio_data_ == nullptr || audio_size_ == 0) {
    ESP_LOGW(TAG, "No audio data available for: %s", alarm_filename_.c_str());
    ESP_LOGW(TAG, "Audio data pointer: %p, size: %d", audio_data_, audio_size_);
    
    // Send a test response to verify the server is working
    String test_response = "Audio Server Test Response\n";
    test_response += "File requested: " + String(alarm_filename_.c_str()) + "\n";
    test_response += "Audio data: " + String(audio_data_ ? "Available" : "NULL") + "\n";
    test_response += "Audio size: " + String(audio_size_) + " bytes\n";
    test_response += "Check compilation logs for embedding status\n";
    test_response += "Mode: Offline capable\n";
    
    request->send(200, "text/plain", test_response);
    return;
  }
  
  String content_type = get_content_type_(alarm_filename_);
  
  ESP_LOGI(TAG, "Serving looped audio stream: %s (%d bytes, %s)", 
           alarm_filename_.c_str(), audio_size_, content_type.c_str());
  
  // Check if file is short (< 60 seconds of audio roughly)
  bool should_loop = audio_size_ < 500000; // Files under ~500KB will loop
  if (should_loop) {
    ESP_LOGI(TAG, "File is short (%d bytes), enabling infinite looping for alarm", audio_size_);
  }
  
  // For looped streaming, we don't specify a content length (infinite stream)
  AsyncWebServerResponse *response = request->beginResponse(
    content_type, 0, // No content length = infinite stream
    [this, should_loop](uint8_t *buffer, size_t maxLen, size_t index) -> size_t {
      if (audio_data_ == nullptr || audio_size_ == 0) {
        return 0;
      }
      
      size_t to_copy;
      if (should_loop) {
        // Loop the file infinitely
        size_t file_pos = index % audio_size_;  // Wrap around when we reach the end
        
        // Handle case where we need to wrap around within this buffer
        if (file_pos + maxLen <= audio_size_) {
          // Simple case - all data fits within current loop iteration
          to_copy = maxLen;
          memcpy(buffer, audio_data_ + file_pos, to_copy);
        } else {
          // We need to wrap around - copy end of file + beginning
          size_t end_bytes = audio_size_ - file_pos;
          size_t start_bytes = maxLen - end_bytes;
          
          // Copy end of file
          memcpy(buffer, audio_data_ + file_pos, end_bytes);
          // Copy beginning of file
          if (start_bytes > 0) {
            memcpy(buffer + end_bytes, audio_data_, start_bytes);
          }
          to_copy = maxLen;
        }
        
        ESP_LOGV(TAG, "Looped streaming: index=%d, file_pos=%d, copied=%d bytes", 
                 index, file_pos, to_copy);
      } else {
        // Play file once (for longer files)
        if (index >= audio_size_) {
          ESP_LOGD(TAG, "Single-play stream complete at index %d", index);
          return 0; // End of stream
        }
        
        to_copy = min(maxLen, audio_size_ - index);
        memcpy(buffer, audio_data_ + index, to_copy);
        
        ESP_LOGD(TAG, "Single-play streaming: bytes %d-%d (%d bytes)", 
                 index, index + to_copy - 1, to_copy);
      }
      
      return to_copy;
    }
  );
  
  response->addHeader("Accept-Ranges", "bytes");
  response->addHeader("Cache-Control", "no-cache");
  
  if (should_loop) {
    // For infinite streams, don't set content-length
    response->addHeader("Transfer-Encoding", "chunked");
    ESP_LOGI(TAG, "Serving as infinite looped stream (no content-length)");
  } else {
    // For single-play files, set content length
    response->addHeader("Content-Length", String(audio_size_));
    ESP_LOGI(TAG, "Serving as single-play stream (%d bytes)", audio_size_);
  }
  
  request->send(response);
}

String SimpleAudioServerComponent::get_content_type_(const std::string& filename) {
  String lower_filename = String(filename.c_str());
  lower_filename.toLowerCase();
  
  if (lower_filename.endsWith(".mp3")) return "audio/mpeg";
  if (lower_filename.endsWith(".wav")) return "audio/wav";
  if (lower_filename.endsWith(".flac")) return "audio/flac";
  if (lower_filename.endsWith(".aac")) return "audio/aac";
  if (lower_filename.endsWith(".m4a")) return "audio/mp4";
  if (lower_filename.endsWith(".ogg")) return "audio/ogg";
  
  return "audio/mpeg"; // Default fallback
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

void SimpleAudioServerComponent::loop() {
  static bool first_loop = true;
  if (first_loop) {
    ESP_LOGD(TAG, "loop() method is being called - component is active");
    first_loop = false;
  }
}

void SimpleAudioServerComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "SOAS Audio File Server (Offline Mode):");
  ESP_LOGCONFIG(TAG, "  Port: %d", port_);
  ESP_LOGCONFIG(TAG, "  Alarm file: %s", alarm_filename_.c_str());
  ESP_LOGCONFIG(TAG, "  Audio data: %s", (audio_data_ && audio_size_ > 0) ? "Available" : "Not found");
  if (audio_data_ && audio_size_ > 0) {
    ESP_LOGCONFIG(TAG, "  File size: %d bytes", audio_size_);
    ESP_LOGCONFIG(TAG, "  Looping: %s", audio_size_ < 500000 ? "YES (infinite)" : "NO (single play)");
  }
  ESP_LOGCONFIG(TAG, "  Network independence: FULL (works without any network)");
  ESP_LOGCONFIG(TAG, "  SOAS stream URL: http://127.0.0.1:%d/%s", port_, alarm_filename_.c_str());
  ESP_LOGCONFIG(TAG, "  WiFi status: %s (not required)", 
                WiFi.status() == WL_CONNECTED ? "Connected" : "Offline");
}

}  // namespace simple_audio_server
}  // namespace esphome
