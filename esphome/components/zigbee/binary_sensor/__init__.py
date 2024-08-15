import esphome.codegen as cg
from esphome.components import binary_sensor
import esphome.config_validation as cv
from esphome.const import (
    CONF_DEVICE_CLASS,
    CONF_ENTITY_CATEGORY,
    CONF_ICON,
    CONF_LAMBDA,
    CONF_SOURCE_ID,
)
from esphome.core.entity_helpers import inherit_property_from

zigbee_on_off_ns = cg.esphome_ns.namespace("zigbee")
ZigbeeBinarySensor = zigbee_on_off_ns.class_(
    "ZigbeeBinarySensor", binary_sensor.BinarySensor, cg.Component
)

AUTO_LOAD = ["zigbee"]

CONFIG_SCHEMA = (
    binary_sensor.binary_sensor_schema(ZigbeeBinarySensor)
    .extend(
        {
            cv.Optional(CONF_LAMBDA): cv.returning_lambda,
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
)


async def to_code(config):
    pass
    var = await binary_sensor.new_binary_sensor(config)
    await cg.register_component(var, config)

    # source = await cg.get_variable(config[CONF_SOURCE_ID])
    # cg.add(var.set_source(source))
