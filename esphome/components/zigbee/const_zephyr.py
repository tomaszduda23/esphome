# These schema keys live in esphome.const so entity base
# schemas can use them without importing this package.
from esphome.const import (  # noqa: F401  # pylint: disable=unused-import
    CONF_ZIGBEE_BINARY_SENSOR,
    CONF_ZIGBEE_ID,
    CONF_ZIGBEE_NUMBER,
    CONF_ZIGBEE_SENSOR,
    CONF_ZIGBEE_SWITCH,
)

MAX_EP_NUMBER_ZEPHYR = 8
CONF_IEEE802154_VENDOR_OUI = "ieee802154_vendor_oui"

# Keys for CORE.data storage
KEY_EP_NUMBER = "ep_number"

# External ZBOSS SDK types (just strings for codegen)
ZB_ZCL_BASIC_ATTRS_EXT_T = "zb_zcl_basic_attrs_ext_t"
ZB_ZCL_IDENTIFY_ATTRS_T = "zb_zcl_identify_attrs_t"

# Cluster IDs
ZB_ZCL_CLUSTER_ID_BASIC = "ZB_ZCL_CLUSTER_ID_BASIC"
ZB_ZCL_CLUSTER_ID_IDENTIFY = "ZB_ZCL_CLUSTER_ID_IDENTIFY"
ZB_ZCL_CLUSTER_ID_BINARY_INPUT = "ZB_ZCL_CLUSTER_ID_BINARY_INPUT"
ZB_ZCL_CLUSTER_ID_ANALOG_INPUT = "ZB_ZCL_CLUSTER_ID_ANALOG_INPUT"
ZB_ZCL_CLUSTER_ID_BINARY_OUTPUT = "ZB_ZCL_CLUSTER_ID_BINARY_OUTPUT"
ZB_ZCL_CLUSTER_ID_ANALOG_OUTPUT = "ZB_ZCL_CLUSTER_ID_ANALOG_OUTPUT"

# sdk-nrf 3.0 and newer have no Zigbee stack; Nordic ships it as the ncs-zigbee add-on,
# whose releases each build on one sdk-nrf version
NCS_ZIGBEE_URL = "https://github.com/nrfconnect/ncs-zigbee"
NCS_ZIGBEE_COMMITS = {
    "3.4.0": "bc7863e99cbbe121d8755bc49586e977cd46d032",  # v1.4.0, ZBOSS 4.2
    "3.4.1": "bc7863e99cbbe121d8755bc49586e977cd46d032",  # v1.4.0, ZBOSS 4.2
    "3.4.2": "c99ea1a3f65235045579ff57452497d386925b5d",  # v2.0.0, ZBOSS 5
}
