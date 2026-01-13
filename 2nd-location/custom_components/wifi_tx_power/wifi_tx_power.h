#pragma once

#include "esphome/core/component.h"
#include "esphome/components/select/select.h"
#include "esphome/core/preferences.h"
#include <vector>
#include <string>

namespace esphome {
namespace wifi_tx_power {

class WiFiTxPowerSelect : public select::Select, public Component {
 public:
  void setup() override;
  void dump_config() override;
  void control(const std::string &value) override;
  void set_restore_value() { this->restore_value_ = true; }
  void set_options(const std::vector<std::string> &options);
  void set_initial_option(const std::string &initial_option);

 protected:
  ESPPreferenceObject pref_;
  std::vector<std::string> options_;
  std::string initial_option_;
  bool restore_value_{false};
};

}  // namespace wifi_tx_power
}  // namespace esphome