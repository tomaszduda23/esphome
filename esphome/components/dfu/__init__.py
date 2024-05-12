import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import output
from esphome.const import (
    CONF_ID,
)

from esphome.components.zephyr import zephyr_add_prj_conf

dfu_ns = cg.esphome_ns.namespace("dfu")
DeviceFirmwareUpdate = dfu_ns.class_("DeviceFirmwareUpdate", cg.Component)

CONF_RESET_OUTPUT = "reset_output"

CONFIG_SCHEMA = cv.All(
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(DeviceFirmwareUpdate),
            cv.Required(CONF_RESET_OUTPUT): cv.use_id(output.BinaryOutput),
        }
    ).extend(cv.COMPONENT_SCHEMA),
    cv.only_on_nrf52,
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    reset_output = await cg.get_variable(config[CONF_RESET_OUTPUT])
    cg.add(var.set_reset_output(reset_output))
    zephyr_add_prj_conf("CDC_ACM_DTE_RATE_CALLBACK_SUPPORT", True)
    await cg.register_component(var, config)