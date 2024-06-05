import random
import esphome.config_validation as cv
import esphome.codegen as cg
from esphome.components.zephyr import zephyr_add_prj_conf
from esphome.const import CONF_ID
from esphome.cpp_generator import MockObj
from esphome.core import CORE

zigbee_ns = cg.esphome_ns.namespace("zigbee")
Zigbee = zigbee_ns.class_("Zigbee", cg.Component)

KEY_ZIGBEE = "zigbee"
KEY_EP = "ep"


def zigbee_set_core_data(config):
    CORE.data[KEY_ZIGBEE] = {}
    CORE.data[KEY_ZIGBEE][KEY_EP] = []
    return config


CONFIG_SCHEMA = cv.All(
    cv.Schema(
        {
            cv.GenerateID(CONF_ID): cv.declare_id(Zigbee),
        }
    ).extend(cv.COMPONENT_SCHEMA),
    zigbee_set_core_data,
)


async def to_code(config):
    cg.add_global(
        MockObj(
            f"ZBOSS_DECLARE_DEVICE_CTX_EP_VA(zb_device_ctx, &{', &'.join(CORE.data[KEY_ZIGBEE][KEY_EP])})"
        )
    )
    cg.add(MockObj("ZB_AF_REGISTER_DEVICE_CTX(&zb_device_ctx)"))

    # zigbee
    zephyr_add_prj_conf("ZIGBEE", True)
    zephyr_add_prj_conf("ZIGBEE_APP_UTILS", True)
    zephyr_add_prj_conf("ZIGBEE_ROLE_END_DEVICE", True)

    zephyr_add_prj_conf("ZIGBEE_CHANNEL_SELECTION_MODE_MULTI", True)

    # TODO zigbee2mqtt do not update configuration of device without this
    zephyr_add_prj_conf("IEEE802154_VENDOR_OUI_ENABLE", True)
    random_number = random.randint(0x000000, 0xFFFFFF)
    zephyr_add_prj_conf("IEEE802154_VENDOR_OUI", random_number)

    # crypto
    zephyr_add_prj_conf("CRYPTO", True)

    # networking
    zephyr_add_prj_conf("NET_IPV6", False)
    zephyr_add_prj_conf("NET_IP_ADDR_CHECK", False)
    zephyr_add_prj_conf("NET_UDP", False)
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
