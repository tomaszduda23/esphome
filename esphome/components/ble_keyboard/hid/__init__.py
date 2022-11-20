import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID, CONF_TYPE
from esphome.components import hid
from esphome.components.hid import (
    CONF_KEYBOARD,
    CONF_MEDIA_KEYS,
    CONF_COMPOSITE,
)

CODEOWNERS = ["@tomaszduda23"]
AUTO_LOAD = ["keyboard"]
CONFLICTS_WITH = [
    "esp32_ble",
    "esp32_ble_tracker",
    "esp32_ble_beacon",
    "esp32_ble_server",
    "esp32_ble_client",
]

ble_keyboard_ns = cg.esphome_ns.namespace("ble_keyboard")
BLEKeyboard = ble_keyboard_ns.class_("BLEKeyboard", hid.HIDDevice)


def add_keyboard_and_media_kyes(config):
    config[CONF_TYPE] = {CONF_COMPOSITE: [CONF_KEYBOARD, CONF_MEDIA_KEYS]}
    return config


CONFIG_SCHEMA = cv.All(
    cv.ENTITY_BASE_SCHEMA.extend(
        {
            cv.GenerateID(): cv.declare_id(BLEKeyboard),
        }
    ).extend(cv.polling_component_schema("10s")),
    add_keyboard_and_media_kyes,
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    cg.add_build_flag("-DUSE_NIMBLE=1")
    cg.add_library("h2zero/NimBLE-Arduino", "1.4.1", None)
    cg.add_library("t-vk/ESP32 BLE Keyboard", "0.3.2", None)
