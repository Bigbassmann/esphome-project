#include "wifi_tx_power.h"
#include "esphome/core/log.h"
#include <esp_wifi.h>

namespace esphome {
namespace wifi_tx_power {

static const char *const TAG = "wifi_tx_power";

void WiFiTxPowerSelect::setup() {
  // Restore and apply saved value on boot
  auto restore = this->traits.get_options().empty() ? "" : this->state;
  if (!restore.empty()) {
    this->control(restore);
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
}

}  // namespace wifi_tx_power
}  // namespace esphome
