#pragma once
#include "esphome/core/defines.h"
#ifdef USE_ZEPHYR_CDC_ACM_RATE_CALLBACK

#include "esphome/core/component.h"
#include "esphome/core/helpers.h"
#include <zephyr/device.h>

namespace esphome::zephyr {

class CdcAcm final : public Component {
 public:
  CdcAcm();
  void setup() override;
  template<typename F> void add_on_rate_callback(F &&callback) { this->rate_callbacks_.add(std::forward<F>(callback)); }
  /// Called by the USB stack when the host changes the baud rate.
  static void dte_rate_callback(const device *device, uint32_t rate);

 protected:
  CallbackManager<void(const device *, uint32_t)> rate_callbacks_;
};

extern CdcAcm *global_cdc_acm;  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

}  // namespace esphome::zephyr

#endif
