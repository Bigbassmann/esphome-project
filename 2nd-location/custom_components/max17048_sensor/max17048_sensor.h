#pragma once

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/i2c/i2c.h"

namespace esphome {
namespace max17048_sensor {

class MAX17048Sensor : public PollingComponent, public i2c::I2CDevice {
 public:
  void set_voltage_sensor(sensor::Sensor *voltage_sensor) { voltage_sensor_ = voltage_sensor; }
  void set_soc_sensor(sensor::Sensor *soc_sensor) { soc_sensor_ = soc_sensor; }
  void set_rate_sensor(sensor::Sensor *rate_sensor) { rate_sensor_ = rate_sensor; }

  void setup() override {
    uint16_t mode = 0x4000;  // Quick start value
    uint16_t swapped = __builtin_bswap16(mode);  // To big-endian
    if (this->write_register(0x06, (const uint8_t *) &swapped, 2) != ESP_OK) {
      ESP_LOGW("MAX17048", "Quick start failed");  // Optional, log warning
    }
  }

  void update() override {
  uint16_t raw;
  if (this->voltage_sensor_ != nullptr) {
    if (this->read_register(0x02, (uint8_t *) &raw, 2) == ESP_OK) {
      ESP_LOGD("MAX17048", "Raw VCELL: 0x%04X", raw);
      float voltage = __builtin_bswap16(raw) * 0.000078125f;  // Correct: 78.125 μV/LSB
      this->voltage_sensor_->publish_state(voltage);
    } else {
      this->voltage_sensor_->publish_state(NAN);
    }
  }

    if (this->soc_sensor_ != nullptr) {
      if (this->read_register(0x04, (uint8_t *) &raw, 2) == ESP_OK) {
        raw = __builtin_bswap16(raw);
        float soc = (raw >> 8) + (raw & 0xFF) / 256.0f;  // Integer + fraction
        this->soc_sensor_->publish_state(soc);
      } else {
        this->soc_sensor_->publish_state(NAN);
      }
    }

    if (this->rate_sensor_ != nullptr) {
      if (this->read_register(0x16, (uint8_t *) &raw, 2) == ESP_OK) {
        int16_t signed_raw = (int16_t) __builtin_bswap16(raw);
        float rate = signed_raw * 0.00208f;  // Signed * 0.208 %
        this->rate_sensor_->publish_state(rate);
      } else {
        this->rate_sensor_->publish_state(NAN);
      }
    }
  }

 protected:
  sensor::Sensor *voltage_sensor_{nullptr};
  sensor::Sensor *soc_sensor_{nullptr};
  sensor::Sensor *rate_sensor_{nullptr};
};

}  // namespace max17048_sensor
}  // namespace esphome