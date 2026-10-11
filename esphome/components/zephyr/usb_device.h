#pragma once
#ifdef USE_ZEPHYR
#if defined(CONFIG_USB_DEVICE_STACK) || defined(CONFIG_USB_DEVICE_STACK_NEXT)

namespace esphome::zephyr {

/// Start the USB device. Safe to call more than once.
void usb_device_enable();

}  // namespace esphome::zephyr

#endif
#endif
