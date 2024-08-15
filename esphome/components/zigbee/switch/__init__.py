import esphome.codegen as cg
from esphome.components import output, switch
import esphome.config_validation as cv
from esphome.const import CONF_OUTPUT
from esphome.core import CORE
from esphome.cpp_generator import MockObj

zigbee_on_off_ns = cg.esphome_ns.namespace("zigbee_on_off")
ZigbeeOnOffSwitch = zigbee_on_off_ns.class_("ZigbeeOnOff", switch.Switch, cg.Component)

cluster_attributes = zigbee_on_off_ns.class_("cluster_attributes_t")

ZB_ZCL_DECLARE_BASIC_ATTRIB_LIST_EXT = cg.global_ns.class_(
    "ZB_ZCL_DECLARE_BASIC_ATTRIB_LIST_EXT", cg.Component
)
ZB_ZCL_DECLARE_IDENTIFY_ATTRIB_LIST = cg.global_ns.class_(
    "ZB_ZCL_DECLARE_IDENTIFY_ATTRIB_LIST", cg.Component
)
ZB_ZCL_DECLARE_GROUPS_ATTRIB_LIST = cg.global_ns.class_(
    "ZB_ZCL_DECLARE_GROUPS_ATTRIB_LIST", cg.Component
)
ZB_ZCL_DECLARE_SCENES_ATTRIB_LIST = cg.global_ns.class_(
    "ZB_ZCL_DECLARE_SCENES_ATTRIB_LIST", cg.Component
)
ZB_ZCL_DECLARE_ON_OFF_ATTRIB_LIST = cg.global_ns.class_(
    "ZB_ZCL_DECLARE_ON_OFF_ATTRIB_LIST", cg.Component
)
ZB_HA_DECLARE_ON_OFF_OUTPUT_CLUSTER_LIST = cg.global_ns.class_(
    "ZB_HA_DECLARE_ON_OFF_OUTPUT_CLUSTER_LIST", cg.Component
)
ZB_EP = cg.global_ns.class_("ZB_EP", cg.Component)

AUTO_LOAD = ["zigbee"]

CONF_CLUSTER_ATTRIBUTES = "cluster_attributes"

CONF_BASIC_ATTRIB_LIST_EXT = "basic_attrib_list_ext"
CONF_IDENTIFY_ATTR_LIST = "identify_attr_list"
CONF_GROUPS_ATTR_LIST = "groups_attr_list"
CONF_SCENES_ATTR_LIST = "scenes_attr_list"
CONF_ON_OFF_ATTR_LIST = "on_off_attr_list"
CONF_ON_OFF_OUTPUT_CLUSTER_LIST = "on_off_output_cluster_list"
CONF_ZB_EP = "zb_ep"

zigbee_ns = cg.esphome_ns.namespace("zigbee")
Zigbee = zigbee_ns.class_("Zigbee", cg.Component)

CONF_ZIGBEE_ID = "zigbee_id"

CONFIG_SCHEMA = (
    switch.switch_schema(ZigbeeOnOffSwitch)
    .extend(
        {
            cv.GenerateID(CONF_ZIGBEE_ID): cv.use_id(Zigbee),
            cv.Required(CONF_OUTPUT): cv.use_id(output.BinaryOutput),
            cv.GenerateID(CONF_CLUSTER_ATTRIBUTES): cv.declare_id(cluster_attributes),
            cv.GenerateID(CONF_BASIC_ATTRIB_LIST_EXT): cv.declare_id(
                ZB_ZCL_DECLARE_BASIC_ATTRIB_LIST_EXT
            ),
            cv.GenerateID(CONF_IDENTIFY_ATTR_LIST): cv.declare_id(
                ZB_ZCL_DECLARE_IDENTIFY_ATTRIB_LIST
            ),
            cv.GenerateID(CONF_GROUPS_ATTR_LIST): cv.declare_id(
                ZB_ZCL_DECLARE_GROUPS_ATTRIB_LIST
            ),
            cv.GenerateID(CONF_SCENES_ATTR_LIST): cv.declare_id(
                ZB_ZCL_DECLARE_SCENES_ATTRIB_LIST
            ),
            cv.GenerateID(CONF_ON_OFF_ATTR_LIST): cv.declare_id(
                ZB_ZCL_DECLARE_ON_OFF_ATTRIB_LIST
            ),
            cv.GenerateID(CONF_ON_OFF_OUTPUT_CLUSTER_LIST): cv.declare_id(
                ZB_HA_DECLARE_ON_OFF_OUTPUT_CLUSTER_LIST
            ),
            cv.GenerateID(CONF_ZB_EP): cv.declare_id(ZB_EP),
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
)

KEY_ZIGBEE = "zigbee"
KEY_EP = "ep"


async def to_code(config):
    cg.add_global(MockObj(f"{cluster_attributes} {config[CONF_CLUSTER_ATTRIBUTES]}"))

    # Declare attributes

    # basic cluster attributes data
    cg.add_global(
        MockObj(
            f"ZB_ZCL_DECLARE_BASIC_ATTRIB_LIST_EXT({config[CONF_BASIC_ATTRIB_LIST_EXT]}, &{config[CONF_CLUSTER_ATTRIBUTES]}.basic_attr.zcl_version, &{config[CONF_CLUSTER_ATTRIBUTES]}.basic_attr.app_version, &{config[CONF_CLUSTER_ATTRIBUTES]}.basic_attr.stack_version, &{config[CONF_CLUSTER_ATTRIBUTES]}.basic_attr.hw_version, {config[CONF_CLUSTER_ATTRIBUTES]}.basic_attr.mf_name, {config[CONF_CLUSTER_ATTRIBUTES]}.basic_attr.model_id, {config[CONF_CLUSTER_ATTRIBUTES]}.basic_attr.date_code, &{config[CONF_CLUSTER_ATTRIBUTES]}.basic_attr.power_source, {config[CONF_CLUSTER_ATTRIBUTES]}.basic_attr.location_id, &{config[CONF_CLUSTER_ATTRIBUTES]}.basic_attr.ph_env, {config[CONF_CLUSTER_ATTRIBUTES]}.basic_attr.sw_ver)"
        )
    )

    # identify cluster attributes data
    cg.add_global(
        MockObj(
            f"ZB_ZCL_DECLARE_IDENTIFY_ATTRIB_LIST({config[CONF_IDENTIFY_ATTR_LIST]}, &{config[CONF_CLUSTER_ATTRIBUTES]}.identify_attr.identify_time)"
        )
    )

    # groups cluster attributes data
    cg.add_global(
        MockObj(
            f"ZB_ZCL_DECLARE_GROUPS_ATTRIB_LIST({config[CONF_GROUPS_ATTR_LIST]}, &{config[CONF_CLUSTER_ATTRIBUTES]}.groups_attr.name_support)"
        )
    )

    # scenes cluster attribute data
    cg.add_global(
        MockObj(
            f"ZB_ZCL_DECLARE_SCENES_ATTRIB_LIST({config[CONF_SCENES_ATTR_LIST]}, &{config[CONF_CLUSTER_ATTRIBUTES]}.scenes_attr.scene_count, &{config[CONF_CLUSTER_ATTRIBUTES]}.scenes_attr.current_scene, &{config[CONF_CLUSTER_ATTRIBUTES]}.scenes_attr.current_group, &{config[CONF_CLUSTER_ATTRIBUTES]}.scenes_attr.scene_valid, &{config[CONF_CLUSTER_ATTRIBUTES]}.scenes_attr.name_support)"
        )
    )

    # on/off cluster attributes data
    cg.add_global(
        MockObj(
            f"ZB_ZCL_DECLARE_ON_OFF_ATTRIB_LIST({config[CONF_ON_OFF_ATTR_LIST]}, &{config[CONF_CLUSTER_ATTRIBUTES]}.on_off_attr.on_off)"
        )
    )

    # Declare device
    cg.add_global(
        MockObj(
            f"ZB_HA_DECLARE_ON_OFF_OUTPUT_CLUSTER_LIST({config[CONF_ON_OFF_OUTPUT_CLUSTER_LIST]}, {config[CONF_ON_OFF_ATTR_LIST]}, {config[CONF_BASIC_ATTRIB_LIST_EXT]}, {config[CONF_IDENTIFY_ATTR_LIST]}, {config[CONF_GROUPS_ATTR_LIST]}, {config[CONF_SCENES_ATTR_LIST]})"
        )
    )

    zb_ep = config[CONF_ZB_EP]
    end_point = int(str(zb_ep)[str(zb_ep).find("_id") + 4 :] or "1") + 9

    ep_macro_name = ""
    if str(config[CONF_CLUSTER_ATTRIBUTES])[-1].isdigit():
        ep_macro_name = "ESPHOME_"
    ep_macro_name += "ZB_HA_DECLARE_ON_OFF_OUTPUT_EP"

    cg.add_global(
        MockObj(
            f"{ep_macro_name}({config[CONF_ZB_EP]}, {end_point}, {config[CONF_ON_OFF_OUTPUT_CLUSTER_LIST]})"
        )
    )

    CORE.data[KEY_ZIGBEE][KEY_EP] += [str(config[CONF_ZB_EP])]

    var = await switch.new_switch(config)
    await cg.register_component(var, config)

    output_ = await cg.get_variable(config[CONF_OUTPUT])
    cg.add(var.set_output(output_))
    cg.add(var.set_cluster_attributes(MockObj(f"&{config[CONF_CLUSTER_ATTRIBUTES]}")))
    hub = await cg.get_variable(config[CONF_ZIGBEE_ID])
    cg.add(
        getattr(hub, "add_callback")(
            end_point,
            MockObj(
                f"std::bind(&zigbee_on_off::ZigbeeOnOff::zcl_device_cb, {var}, std::placeholders::_1)"
            ),
        )
    )
