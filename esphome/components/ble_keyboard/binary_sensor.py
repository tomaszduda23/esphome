import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor
from esphome.const import CONF_ID
from .hid import BLEKeyboard

CONF_CONNECTED = "connected"

TYPES = [
    CONF_CONNECTED,
]

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.use_id(BLEKeyboard),
        cv.Optional(CONF_CONNECTED): binary_sensor.binary_sensor_schema(),
    }
).extend(cv.COMPONENT_SCHEMA)


async def setup_conf(config, key, hub):
    if key in config:
        conf = config[key]
        var = await binary_sensor.new_binary_sensor(conf)
        cg.add(getattr(hub, f"set_{key}_binary_sensor")(var))


async def to_code(config):
    hub = await cg.get_variable(config[CONF_ID])
    for key in TYPES:
        await setup_conf(config, key, hub)
