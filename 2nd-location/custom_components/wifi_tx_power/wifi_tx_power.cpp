#include "wifi_tx_power.h"
#include "esphome/core/log.h"
#include <esp_wifi.h>
#include "esphome/core/preferences.h"
#include "esphome/core/helpers.h"
#include <string>

namespace esphome {
namespace wifi_tx_power {

static const char *const TAG = "wifi_tx_power";

void WiFiTxPowerSelect::setup() {
  if (this->restore_value_) {
    this->pref_ = global_preferences->make_preference<uint8_t>(this->get_object_id_hash());
    uint8_t index;
    if (this->pref_.load(&index) && index < this->traits.get_options().size()) {
      std::string restored = this->traits.get_options()[index];
      this->control(restored);
      return;
    }
  }
  if (!this->initial_option_.empty()) {
    this->control(this->initial_option_);
  }
}

void WiFiTxPowerSelect::dump_config() {
  LOG_SELECT(TAG, "WiFi TX Power", this);
}

void WiFiTxPowerSelect::control(const std::string &value) {
  float power_dbm = std::stof(value);
  int8_t power_quarter = static_cast<int8_t>(power_dbm * 4);
  esp_err_t err = esp_wifi_set_max_tx_power(power_quarter);
  if (err == ESP_OK) {
    ESP_LOGI(TAG, "WiFi TX power set to %.1f dBm", power_dbm);
  } else {
    ESP_LOGE(TAG, "Failed to set WiFi TX power: %s", esp_err_to_name(err));
  }
  this->publish_state(value);
  if (this->restore_value_) {
    auto ind = this->index_of(value);
    if (ind.has_value()) {
      uint8_t idx = static_cast<uint8_t>(ind.value());
      this->pref_.save(&idx);
    }
  }
}

void WiFiTxPowerSelect::set_options(const std::vector<std::string> &options) {
  this->options_ = options;
  FixedVector<const char *> fixed_options;
  for (const auto &str : options) {
    fixed_options.push_back(str.c_str());
  }
  this->traits.set_options(fixed_options);
}

void WiFiTxPowerSelect::set_initial_option(const std::string &initial_option) {
  this->initial_option_ = initial_option;
}

}  // namespace wifi_tx_power
}  // namespace esphome