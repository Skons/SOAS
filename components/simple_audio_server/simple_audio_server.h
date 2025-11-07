#pragma once

#include "esphome/core/defines.h"

#include "esphome/components/web_server_base/web_server_base.h"
#include "esphome/core/component.h"

namespace esphome {
namespace web_server {

//class SimpleAudioServerComponent : public Component {
class SimpleAudioServerComponent : public AsyncWebHandler, public Component {
 public:
  void setup() override;
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::AFTER_WIFI; }

  void set_alarm_file(const std::string& filename);
  void set_audio_data(const uint8_t* data, size_t size);

 protected:
  friend class SASRequestHandler;
 private:
  std::string alarm_filename_;

  const uint8_t* audio_data_{nullptr};
  size_t audio_size_{0};

  std::string get_content_type_(const std::string& filename);

  static const size_t STREAM_BUFFER_SIZE = 1024;
};

}  // namespace web_server
}  // namespace esphome
