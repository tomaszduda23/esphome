#include "zigbee_on_off.h"
#include "esphome/core/log.h"
#include <zephyr/settings/settings.h>

extern "C" {
#include <zboss_api.h>
#include <zboss_api_addons.h>
#include <zb_nrf_platform.h>
#include <zigbee/zigbee_app_utils.h>
#include <zb_error_to_string.h>
}

namespace esphome {
namespace zigbee_on_off {

#define HA_ESP_LIGHT_ENDPOINT 14
#define HA_ESP_LIGHT_ENDPOINT2 15
#define BULB_INIT_BASIC_LOCATION_DESC "TODO"
#define BULB_INIT_BASIC_PH_ENV ZB_ZCL_BASIC_ENV_UNSPECIFIED
#define BULB_INIT_BASIC_MODEL_ID "TODO"
#define BULB_INIT_BASIC_DATE_CODE "20200329"
#define BULB_INIT_BASIC_APP_VERSION 00
#define BULB_INIT_BASIC_HW_VERSION 00
#define BULB_INIT_BASIC_STACK_VERSION 00
#define BULB_INIT_BASIC_MANUF_NAME "esphome"

static const char *const TAG = "zigbee_on_off.switch";

void ZigbeeOnOff::dump_config() { LOG_SWITCH("", "Zigbee Switch", this); }

void ZigbeeOnOff::setup() {
  cluster_attributes_->basic_attr.zcl_version = ZB_ZCL_VERSION;
  cluster_attributes_->basic_attr.app_version = BULB_INIT_BASIC_APP_VERSION;
  cluster_attributes_->basic_attr.stack_version = BULB_INIT_BASIC_STACK_VERSION;
  cluster_attributes_->basic_attr.hw_version = BULB_INIT_BASIC_HW_VERSION;

  ZB_ZCL_SET_STRING_VAL(cluster_attributes_->basic_attr.mf_name, BULB_INIT_BASIC_MANUF_NAME,
                        ZB_ZCL_STRING_CONST_SIZE(BULB_INIT_BASIC_MANUF_NAME));

  ZB_ZCL_SET_STRING_VAL(cluster_attributes_->basic_attr.model_id, BULB_INIT_BASIC_MODEL_ID,
                        ZB_ZCL_STRING_CONST_SIZE(BULB_INIT_BASIC_MODEL_ID));

  ZB_ZCL_SET_STRING_VAL(cluster_attributes_->basic_attr.date_code, BULB_INIT_BASIC_DATE_CODE,
                        ZB_ZCL_STRING_CONST_SIZE(BULB_INIT_BASIC_DATE_CODE));

  cluster_attributes_->basic_attr.power_source = ZB_ZCL_BASIC_POWER_SOURCE_DC_SOURCE;

  ZB_ZCL_SET_STRING_VAL(cluster_attributes_->basic_attr.location_id, BULB_INIT_BASIC_LOCATION_DESC,
                        ZB_ZCL_STRING_CONST_SIZE(BULB_INIT_BASIC_LOCATION_DESC));

  cluster_attributes_->basic_attr.ph_env = BULB_INIT_BASIC_PH_ENV;

  /* identify cluster attributes data */
  cluster_attributes_->identify_attr.identify_time = ZB_ZCL_IDENTIFY_IDENTIFY_TIME_DEFAULT_VALUE;
  /* groups cluster attributes data */
  cluster_attributes_->groups_attr.name_support = 0;

  bool initial_state = this->get_initial_state_with_restore_mode().value_or(false);

  if (initial_state) {
    /* on/off cluster attributes data */
    cluster_attributes_->on_off_attr.on_off = ZB_ZCL_ON_OFF_IS_ON;
    this->turn_on();
  } else {
    /* on/off cluster attributes data */
    cluster_attributes_->on_off_attr.on_off = ZB_ZCL_ON_OFF_IS_OFF;
    this->turn_off();
  }
}

void ZigbeeOnOff::write_state(bool state) {
  if (state) {
    this->output_->turn_on();
  } else {
    this->output_->turn_off();
  }
  this->publish_state(state);
}

void ZigbeeOnOff::zcl_device_cb(zb_bufid_t bufid) {
  zb_zcl_device_callback_param_t *p_device_cb_param = ZB_BUF_GET_PARAM(bufid, zb_zcl_device_callback_param_t);
  zb_zcl_device_callback_id_t device_cb_id = p_device_cb_param->device_cb_id;
  zb_uint16_t cluster_id = p_device_cb_param->cb_param.set_attr_value_param.cluster_id;
  zb_uint16_t attr_id = p_device_cb_param->cb_param.set_attr_value_param.attr_id;

  p_device_cb_param->status = RET_OK;

  switch (device_cb_id) {
    /* ZCL set attribute value */
    case ZB_ZCL_SET_ATTR_VALUE_CB_ID:
      if (cluster_id == ZB_ZCL_CLUSTER_ID_ON_OFF) {
        uint8_t value = p_device_cb_param->cb_param.set_attr_value_param.values.data8;
        ESP_LOGI(TAG, "on/off attribute setting to %hd", value);

        if (attr_id == ZB_ZCL_ATTR_ON_OFF_ON_OFF_ID) {
          write_state((zb_bool_t) value);
        }
      } else {
        /* other clusters attribute handled here */
        ESP_LOGI(TAG, "Unhandled cluster attribute id: %d", cluster_id);
      }
      break;
    default:
      p_device_cb_param->status = RET_ERROR;
      break;
  }

  ESP_LOGD(TAG, "%s status: %hd", __func__, p_device_cb_param->status);
}

}  // namespace zigbee_on_off
}  // namespace esphome
