import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import select
from esphome.const import CONF_ID, CONF_OPTIONS, CONF_OPTIMISTIC, CONF_INITIAL_OPTION, CONF_RESTORE_VALUE

DEPENDENCIES = ['wifi']

wifi_tx_power_ns = cg.esphome_ns.namespace('wifi_tx_power')
WiFiTxPowerSelect = wifi_tx_power_ns.class_('WiFiTxPowerSelect', select.Select)

def validate_initial_option(config):
    if CONF_INITIAL_OPTION in config and config[CONF_INITIAL_OPTION] not in config[CONF_OPTIONS]:
        raise cv.Invalid(
            "The initial_option '{}' is not one of the options. ".format(config[CONF_INITIAL_OPTION]) +
            "Please choose one of {}.".format(', '.join("'{}'".format(x) for x in config[CONF_OPTIONS]))
        )
    return config

CONFIG_SCHEMA = select.select_schema(WiFiTxPowerSelect).extend({
    cv.Required(CONF_OPTIONS): cv.All(cv.ensure_list(cv.string_strict), cv.Length(min=1)),
    cv.Optional(CONF_INITIAL_OPTION): cv.string_strict,
    cv.Optional(CONF_OPTIMISTIC, default=True): cv.boolean,
    cv.Optional(CONF_RESTORE_VALUE, default=True): cv.boolean,
})

CONFIG_SCHEMA = cv.All(CONFIG_SCHEMA, validate_initial_option)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await select.register_select(var, config, options=config[CONF_OPTIONS])
    if CONF_INITIAL_OPTION in config:
        cg.add(var.set_initial_option(config[CONF_INITIAL_OPTION]))
    if config[CONF_RESTORE_VALUE]:
        cg.add(var.set_restore_value())