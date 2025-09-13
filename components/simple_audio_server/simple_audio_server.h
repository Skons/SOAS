#pragma once

#include "esphome/core/component.h"
#include "esphome/core/log.h"
#include <ESPAsyncWebServer.h>
#include <WiFi.h>

namespace esphome {
namespace simple_audio_server {

static const char *const TAG = "simple_audio_server";

class SimpleAudioServerComponent : public Component {
 public:
  SimpleAudioServerComponent();
  
  void setup() override;
  void loop() override;
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::AFTER_WIFI; }
  
  void set_alarm_file(const std::string& filename);
  void set_port(uint16_t port) { port_ = port; }
  void set_audio_data(const uint8_t* data, size_t size);

 private:
  std::unique_ptr<AsyncWebServer> server_;
  std::string alarm_filename_;
  uint16_t port_{8080};
  
  // Embedded audio data
  const uint8_t* audio_data_{nullptr};
  size_t audio_size_{0};
  
  void handle_audio_stream_(AsyncWebServerRequest *request);
  void start_server();
  String get_content_type_(const std::string& filename);
  
  // Stream buffer size
  static const size_t STREAM_BUFFER_SIZE = 1024;
};

}  // namespace simple_audio_server
}  // namespace esphome
