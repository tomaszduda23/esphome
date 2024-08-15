#pragma once

#include "esphome/core/component.h"
#include "esphome/components/switch/switch.h"
#include "esphome/components/output/binary_output.h"
extern "C" {
#include <zboss_api.h>
#include <zboss_api_addons.h>
}

#define ESPHOME_ZB_ZCL_DECLARE_ON_OFF_OUTPUT_SIMPLE_DESC(ep_name, ep_id, in_clust_num, out_clust_num) \
  ZB_AF_SIMPLE_DESC_TYPE(in_clust_num, out_clust_num) \
  simple_desc_##ep_name = {ep_id, \
                           ZB_AF_HA_PROFILE_ID, \
                           ZB_HA_ON_OFF_OUTPUT_DEVICE_ID, \
                           ZB_HA_DEVICE_VER_ON_OFF_OUTPUT, \
                           0, \
                           in_clust_num, \
                           out_clust_num, \
                           {ZB_ZCL_CLUSTER_ID_BASIC, ZB_ZCL_CLUSTER_ID_IDENTIFY, ZB_ZCL_CLUSTER_ID_ON_OFF, \
                            ZB_ZCL_CLUSTER_ID_SCENES, ZB_ZCL_CLUSTER_ID_GROUPS}}

#define ESPHOME_ZB_HA_DECLARE_ON_OFF_OUTPUT_EP(ep_name, ep_id, cluster_list) \
  ESPHOME_ZB_ZCL_DECLARE_ON_OFF_OUTPUT_SIMPLE_DESC(ep_name, ep_id, ZB_HA_ON_OFF_OUTPUT_IN_CLUSTER_NUM, \
                                                   ZB_HA_ON_OFF_OUTPUT_OUT_CLUSTER_NUM); \
  ZBOSS_DEVICE_DECLARE_REPORTING_CTX(reporting_info##ep_name, ZB_HA_ON_OFF_OUTPUT_REPORT_ATTR_COUNT); \
  ZB_AF_DECLARE_ENDPOINT_DESC(ep_name, ep_id, ZB_AF_HA_PROFILE_ID, 0, NULL, \
                              ZB_ZCL_ARRAY_SIZE(cluster_list, zb_zcl_cluster_desc_t), cluster_list, \
                              (zb_af_simple_desc_1_1_t *) &simple_desc_##ep_name, \
                              ZB_HA_ON_OFF_OUTPUT_REPORT_ATTR_COUNT, reporting_info##ep_name, 0, NULL)

namespace esphome {
namespace zigbee_on_off {

typedef struct {
  zb_zcl_basic_attrs_ext_t basic_attr;
  zb_zcl_identify_attrs_t identify_attr;
  zb_zcl_groups_attrs_t groups_attr;
  zb_zcl_scenes_attrs_t scenes_attr;
  zb_zcl_on_off_attrs_t on_off_attr;
} cluster_attributes_t;

class ZigbeeOnOff : public switch_::Switch, public Component {
 public:
  void set_output(output::BinaryOutput *output) { output_ = output; }
  void set_cluster_attributes(cluster_attributes_t *cluster_attributes) { cluster_attributes_ = cluster_attributes; }

  void setup() override;
  float get_setup_priority() const override { return setup_priority::HARDWARE - 1.0f; }
  void dump_config() override;

  void zcl_device_cb(zb_bufid_t bufid);

 protected:
  void write_state(bool state) override;

  output::BinaryOutput *output_;
  cluster_attributes_t *cluster_attributes_ = nullptr;
};

}  // namespace zigbee_on_off
}  // namespace esphome
