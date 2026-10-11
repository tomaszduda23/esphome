#include "cdc_acm.h"
#ifdef USE_ZEPHYR_CDC_ACM_RATE_CALLBACK
#include "usb_device.h"
#include <zephyr/drivers/uart.h>
#include <zephyr/drivers/uart/cdc_acm.h>

#define DEVICE_AND_COMMA(node_id) DEVICE_DT_GET(node_id),

namespace esphome::zephyr {

CdcAcm::CdcAcm() { global_cdc_acm = this; }

void CdcAcm::setup() {
#ifdef CONFIG_USB_DEVICE_STACK_NEXT
  // The new USB device stack reports baud rate changes through its message callback.
  usb_device_enable();
#elif DT_HAS_COMPAT_STATUS_OKAY(zephyr_cdc_acm_uart)
  const struct device *cdc_dev[] = {DT_FOREACH_STATUS_OKAY(zephyr_cdc_acm_uart, DEVICE_AND_COMMA)};
  for (auto &idx : cdc_dev) {
    // only one global callback can be registered
    cdc_acm_dte_rate_callback_set(idx, CdcAcm::dte_rate_callback);
  }
#endif
}

void CdcAcm::dte_rate_callback(const struct device *device, uint32_t rate) {
  if (global_cdc_acm == nullptr)
    return;
  global_cdc_acm->defer([device, rate]() { global_cdc_acm->rate_callbacks_.call(device, rate); });
}

CdcAcm *global_cdc_acm;  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

}  // namespace esphome::zephyr

#endif
