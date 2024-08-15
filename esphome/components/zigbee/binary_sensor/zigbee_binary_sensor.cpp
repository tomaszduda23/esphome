#include "zigbee_binary_sensor.h"
#include "esphome/core/log.h"
extern "C" {
#include <zboss_api.h>
#include <zboss_api_addons.h>
#include <zb_nrf_platform.h>
#include <zigbee/zigbee_app_utils.h>
#include <zb_error_to_string.h>
}
namespace esphome {
namespace zigbee {

static const char *const TAG = "zigbee.binary_sensor";

#define simple_sensor_ENDPOINT 11

#define BULB_INIT_BASIC_LOCATION_DESC "TODO"
#define BULB_INIT_BASIC_PH_ENV ZB_ZCL_BASIC_ENV_UNSPECIFIED
#define BULB_INIT_BASIC_MODEL_ID "TODO"
#define BULB_INIT_BASIC_DATE_CODE "20200329"
#define BULB_INIT_BASIC_APP_VERSION 00
#define BULB_INIT_BASIC_HW_VERSION 00
#define BULB_INIT_BASIC_STACK_VERSION 00
#define BULB_INIT_BASIC_MANUF_NAME "esphome"

/* light switch device cluster attributes */
typedef struct {
  zb_zcl_basic_attrs_ext_t basic_attr;
  zb_zcl_identify_attrs_t identify_attr;
  // zb_zcl_on_off_attrs_t           on_off_attr;
} switch_device_ctx_t;

static switch_device_ctx_t esp_switch_ctx;

/** @cond internals_doc */
#define ZB_HA_DEVICE_VER_SIMPLE_SENSOR 0 /*!< "Device_name" device version */

#define ZB_HA_SIMPLE_SENSOR_IN_CLUSTER_NUM 3  /*!< Simple Sensor IN (server) clusters number */
#define ZB_HA_SIMPLE_SENSOR_OUT_CLUSTER_NUM 1 /*!< Simple Sensor OUT (client) clusters number */

/** @brief Number of attribute for reporting on Simple Sensor device */
#define ZB_HA_SIMPLE_SENSOR_REPORT_ATTR_COUNT (ZB_ZCL_BINARY_INPUT_REPORT_ATTR_COUNT)

/** @brief Declare cluster list for Simple Sensor device.
    @param cluster_list_name - cluster list variable name
    @param basic_attr_list - attribute list for Basic cluster
    @param identify_attr_list - attribute list for Identify cluster
    @param binary_input_attr_list - attribute list for Binary Input cluster
*/
#define ZB_HA_DECLARE_SIMPLE_SENSOR_CLUSTER_LIST(cluster_list_name, basic_attr_list, identify_attr_list, \
                                                 binary_input_attr_list) \
  zb_zcl_cluster_desc_t cluster_list_name[] = { \
      ZB_ZCL_CLUSTER_DESC(ZB_ZCL_CLUSTER_ID_IDENTIFY, ZB_ZCL_ARRAY_SIZE(identify_attr_list, zb_zcl_attr_t), \
                          (identify_attr_list), ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_MANUF_CODE_INVALID), \
      ZB_ZCL_CLUSTER_DESC(ZB_ZCL_CLUSTER_ID_BASIC, ZB_ZCL_ARRAY_SIZE(basic_attr_list, zb_zcl_attr_t), \
                          (basic_attr_list), ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_MANUF_CODE_INVALID), \
      ZB_ZCL_CLUSTER_DESC(ZB_ZCL_CLUSTER_ID_BINARY_INPUT, ZB_ZCL_ARRAY_SIZE(binary_input_attr_list, zb_zcl_attr_t), \
                          (binary_input_attr_list), ZB_ZCL_CLUSTER_SERVER_ROLE, ZB_ZCL_MANUF_CODE_INVALID), \
      ZB_ZCL_CLUSTER_DESC(ZB_ZCL_CLUSTER_ID_IDENTIFY, 0, NULL, ZB_ZCL_CLUSTER_CLIENT_ROLE, ZB_ZCL_MANUF_CODE_INVALID)}

/** @cond internals_doc */
/** @brief Declare simple descriptor for "Device_name" device
    @param ep_name - endpoint variable name
    @param ep_id - endpoint ID
    @param in_clust_num - number of supported input clusters
    @param out_clust_num - number of supported output clusters
*/
#define ZB_ZCL_DECLARE_SIMPLE_SENSOR_SIMPLE_DESC(ep_name, ep_id, in_clust_num, out_clust_num) \
  ZB_DECLARE_SIMPLE_DESC(in_clust_num, out_clust_num); \
  ZB_AF_SIMPLE_DESC_TYPE(in_clust_num, out_clust_num) \
  simple_desc_##ep_name = {ep_id, \
                           ZB_AF_HA_PROFILE_ID, \
                           ZB_HA_SIMPLE_SENSOR_DEVICE_ID, \
                           ZB_HA_DEVICE_VER_SIMPLE_SENSOR, \
                           0, \
                           in_clust_num, \
                           out_clust_num, \
                           { \
                               ZB_ZCL_CLUSTER_ID_BASIC, \
                               ZB_ZCL_CLUSTER_ID_IDENTIFY, \
                               ZB_ZCL_CLUSTER_ID_BINARY_INPUT, \
                               ZB_ZCL_CLUSTER_ID_IDENTIFY, \
                           }}

/** @brief Declare endpoint for Simple Sensor device
    @param ep_name - endpoint variable name
    @param ep_id - endpoint ID
    @param cluster_list - endpoint cluster list
 */
#define ZB_HA_DECLARE_SIMPLE_SENSOR_EP(ep_name, ep_id, cluster_list) \
  ZB_ZCL_DECLARE_SIMPLE_SENSOR_SIMPLE_DESC(ep_name, ep_id, ZB_HA_SIMPLE_SENSOR_IN_CLUSTER_NUM, \
                                           ZB_HA_SIMPLE_SENSOR_OUT_CLUSTER_NUM); \
  ZBOSS_DEVICE_DECLARE_REPORTING_CTX(reporting_info##device_ctx_name, ZB_HA_SIMPLE_SENSOR_REPORT_ATTR_COUNT); \
  ZB_AF_DECLARE_ENDPOINT_DESC(ep_name, ep_id, ZB_AF_HA_PROFILE_ID, 0, NULL, \
                              ZB_ZCL_ARRAY_SIZE(cluster_list, zb_zcl_cluster_desc_t), cluster_list, \
                              (zb_af_simple_desc_1_1_t *) &simple_desc_##ep_name, \
                              ZB_HA_SIMPLE_SENSOR_REPORT_ATTR_COUNT, reporting_info##device_ctx_name, 0, NULL)

/******************* Declare attributes ************************/

/* basic cluster attributes data */
ZB_ZCL_DECLARE_BASIC_ATTRIB_LIST_EXT(basic_attr_list, &esp_switch_ctx.basic_attr.zcl_version,
                                     &esp_switch_ctx.basic_attr.app_version, &esp_switch_ctx.basic_attr.stack_version,
                                     &esp_switch_ctx.basic_attr.hw_version, esp_switch_ctx.basic_attr.mf_name,
                                     esp_switch_ctx.basic_attr.model_id, esp_switch_ctx.basic_attr.date_code,
                                     &esp_switch_ctx.basic_attr.power_source, esp_switch_ctx.basic_attr.location_id,
                                     &esp_switch_ctx.basic_attr.ph_env, esp_switch_ctx.basic_attr.sw_ver);

/* identify cluster attributes data */
ZB_ZCL_DECLARE_IDENTIFY_ATTRIB_LIST(identify_attr_list, &esp_switch_ctx.identify_attr.identify_time);

/* switch config cluster attributes data */
// zb_uint8_t attr_switch_type =
//     ZB_ZCL_ON_OFF_SWITCH_CONFIGURATION_SWITCH_TYPE_TOGGLE;
// zb_uint8_t attr_switch_actions =
//     ZB_ZCL_ON_OFF_SWITCH_CONFIGURATION_SWITCH_ACTIONS_DEFAULT_VALUE;

// ZB_ZCL_DECLARE_ON_OFF_SWITCH_CONFIGURATION_ATTRIB_LIST(switch_cfg_attr_list, &attr_switch_type,
// &attr_switch_actions);
bool out_of_service = 0; /*!< Out of Service */
bool present_value = 1;
uint8_t status_flags = 0; /*!< Status flags */
ZB_ZCL_DECLARE_BINARY_INPUT_ATTRIB_LIST(binary_input_attr_list, &out_of_service, &present_value, &status_flags);
/********************* Declare device **************************/

// ZB_HA_DECLARE_ON_OFF_SWITCH_CLUSTER_LIST(on_off_switch_clusters, switch_cfg_attr_list, basic_attr_list,
// identify_attr_list);
ZB_HA_DECLARE_SIMPLE_SENSOR_CLUSTER_LIST(simple_sensor_cluster, basic_attr_list, identify_attr_list,
                                         binary_input_attr_list);

// ZB_HA_DECLARE_ON_OFF_SWITCH_EP(on_off_switch_ep, HA_ONOFF_SWITCH_ENDPOINT, on_off_switch_clusters);
ZB_HA_DECLARE_SIMPLE_SENSOR_EP(simple_sensor_ep, simple_sensor_ENDPOINT, simple_sensor_cluster);

// ZB_HA_DECLARE_ON_OFF_SWITCH_CTX(on_off_switch_ctx, on_off_switch_ep);
ZBOSS_DECLARE_DEVICE_CTX_1_EP(simple_sensor_ctx, simple_sensor_ep);

void ZigbeeBinarySensor::setup() {
  esp_switch_ctx.basic_attr.zcl_version = ZB_ZCL_VERSION;
  esp_switch_ctx.basic_attr.app_version = BULB_INIT_BASIC_APP_VERSION;
  esp_switch_ctx.basic_attr.stack_version = BULB_INIT_BASIC_STACK_VERSION;
  esp_switch_ctx.basic_attr.hw_version = BULB_INIT_BASIC_HW_VERSION;

  ZB_ZCL_SET_STRING_VAL(esp_switch_ctx.basic_attr.mf_name, BULB_INIT_BASIC_MANUF_NAME,
                        ZB_ZCL_STRING_CONST_SIZE(BULB_INIT_BASIC_MANUF_NAME));

  ZB_ZCL_SET_STRING_VAL(esp_switch_ctx.basic_attr.model_id, BULB_INIT_BASIC_MODEL_ID,
                        ZB_ZCL_STRING_CONST_SIZE(BULB_INIT_BASIC_MODEL_ID));

  ZB_ZCL_SET_STRING_VAL(esp_switch_ctx.basic_attr.date_code, BULB_INIT_BASIC_DATE_CODE,
                        ZB_ZCL_STRING_CONST_SIZE(BULB_INIT_BASIC_DATE_CODE));

  esp_switch_ctx.basic_attr.power_source = ZB_ZCL_BASIC_POWER_SOURCE_DC_SOURCE;

  ZB_ZCL_SET_STRING_VAL(esp_switch_ctx.basic_attr.location_id, BULB_INIT_BASIC_LOCATION_DESC,
                        ZB_ZCL_STRING_CONST_SIZE(BULB_INIT_BASIC_LOCATION_DESC));

  esp_switch_ctx.basic_attr.ph_env = BULB_INIT_BASIC_PH_ENV;

  esp_switch_ctx.basic_attr.zcl_version = ZB_ZCL_VERSION;
  esp_switch_ctx.basic_attr.power_source = ZB_ZCL_BASIC_POWER_SOURCE_UNKNOWN;

  esp_switch_ctx.identify_attr.identify_time = 0;
  // esp_switch_ctx.on_off_attr.on_off = 0;

  // ZB_AF_REGISTER_DEVICE_CTX(&on_off_switch_ctx);
  ZB_AF_REGISTER_DEVICE_CTX(&simple_sensor_ctx);

  ZB_ZCL_SET_ATTRIBUTE(simple_sensor_ENDPOINT, ZB_ZCL_CLUSTER_ID_BINARY_INPUT, ZB_ZCL_CLUSTER_SERVER_ROLE,
                       ZB_ZCL_ATTR_BINARY_INPUT_PRESENT_VALUE_ID, (zb_uint8_t *) &present_value, ZB_FALSE);

  zigbee_enable();

  if (!this->publish_initial_state_)
    return;

  if (this->f_ != nullptr) {
    this->publish_initial_state(this->f_().value_or(false));
  } else {
    this->publish_initial_state(false);
  }
}
void ZigbeeBinarySensor::loop() {
  if (this->f_ == nullptr)
    return;

  auto s = this->f_();
  if (s.has_value()) {
    this->publish_state(*s);
  }
}
void ZigbeeBinarySensor::dump_config() { LOG_BINARY_SENSOR("", "Zigbee Binary Sensor", this); }

void ZigbeeBinarySensor::update() {
  if (present_value) {
    present_value = 0;
  } else {
    present_value = 1;
  }
  ZB_ZCL_SET_ATTRIBUTE(simple_sensor_ENDPOINT, ZB_ZCL_CLUSTER_ID_BINARY_INPUT, ZB_ZCL_CLUSTER_SERVER_ROLE,
                       ZB_ZCL_ATTR_BINARY_INPUT_PRESENT_VALUE_ID, (zb_uint8_t *) &present_value, ZB_FALSE);
}

}  // namespace zigbee
}  // namespace esphome
