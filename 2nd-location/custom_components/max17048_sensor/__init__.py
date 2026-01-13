import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, i2c
from esphome.const import (
    UNIT_PERCENT,
    UNIT_VOLT,
    DEVICE_CLASS_BATTERY,
    DEVICE_CLASS_VOLTAGE,
    STATE_CLASS_MEASUREMENT,
)

DEPENDENCIES = ["i2c"]

CONF_VOLTAGE = "voltage"
CONF_SOC = "soc"
CONF_RATE = "rate"

max17048_sensor_ns = cg.esphome_ns.namespace("max17048_sensor")
MAX17048Sensor = max17048_sensor_ns.class_("MAX17048Sensor", cg.PollingComponent, i2c.I2CDevice)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(MAX17048Sensor),
        cv.Optional(CONF_VOLTAGE): sensor.sensor_schema(
            unit_of_measurement=UNIT_VOLT,
            accuracy_decimals=3,
            device_class=DEVICE_CLASS_VOLTAGE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_SOC): sensor.sensor_schema(
            unit_of_measurement=UNIT_PERCENT,
            accuracy_decimals=2,
            device_class=DEVICE_CLASS_BATTERY,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_RATE): sensor.sensor_schema(
            unit_of_measurement="%/hr",
            accuracy_decimals=2,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
    }
).extend(cv.polling_component_schema("30s")).extend(i2c.i2c_device_schema(0x36))

async def to_code(config):
    var = cg.new_Pvariable(config[cv.CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)

    if CONF_VOLTAGE in config:
        sens = await sensor.new_sensor(config[CONF_VOLTAGE])
        cg.add(var.set_voltage_sensor(sens))
    if CONF_SOC in config:
        sens = await sensor.new_sensor(config[CONF_SOC])
        cg.add(var.set_soc_sensor(sens))
    if CONF_RATE in config:
        sens = await sensor.new_sensor(config[CONF_RATE])
        cg.add(var.set_rate_sensor(sens))