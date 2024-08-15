#pragma once
#include <map>

#include "esphome/core/component.h"
extern "C" {
#include <zboss_api.h>
}

namespace esphome {
namespace zigbee {

class Zigbee : public Component {
 public:
  void setup() override;
  void add_callback(zb_uint8_t endpoint, std::function<void(zb_bufid_t bufid)> cb) { callbacks_[endpoint] = cb; }
  std::map<zb_uint8_t, std::function<void(zb_bufid_t bufid)>> callbacks_;
};

}  // namespace zigbee
}  // namespace esphome
