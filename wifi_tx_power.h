#pragma once

#include "esphome/core/component.h"
#include "esphome/components/select/select.h"

namespace esphome {
namespace wifi_tx_power {

class WiFiTxPowerSelect : public select::Select, public Component {
 public:
  void setup() override;
  void dump_config() override;

 protected:
  void control(const std::string &value) override;
};

}  // namespace wifi_tx_power
}  // namespace esphome