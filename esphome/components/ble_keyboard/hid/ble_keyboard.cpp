#include "ble_keyboard.h"
#ifdef USE_KEYBOARD
#include "NimBLEDevice.h"
#include "esphome/core/log.h"
#include "esphome/core/application.h"

namespace esphome {
namespace ble_keyboard {

static const char *const TAG = "ble_keyboard";

void KeyboardReport::report() {
  KeyReport keys{};
  keys.modifiers = modifier_;
  std::memcpy(keys.keys, hidcode_, ROLLOVER);
  kbd_.sendReport(&keys);
}

void KeyboardReport::loop() {
#ifdef USE_BINARY_SENSOR
  // those are updated from 2 different threads but it does not matter much
  if (led_control_ && update_led_) {
    update_led_ = false;
    led_control_->publish_numlock(led_indicator_ & 0x1);
    led_control_->publish_capslock(led_indicator_ & 0x2);
    led_control_->publish_scrollock(led_indicator_ & 0x4);
  }
#endif
}

void MediaKeysReport::report() {
  MediaKeyReport keys;
  keys[0] = media_keys_ >> 8;
  keys[1] = media_keys_;
  kbd_.sendReport(&keys);
}

BLEKeyboard::BLEKeyboard() : kbd_report_(*this), media_report_(*this) {}

void BLEKeyboard::setup() {
  setName(App.get_name());
  begin();
  setDelay(0);
  releaseAll();
}

void BLEKeyboard::loop() { kbd_report_.loop(); }

void BLEKeyboard::update() {
#ifdef USE_BINARY_SENSOR
  if (connected_) {
    connected_->publish_state(isConnected());
  }
#endif
}

void BLEKeyboard::dump_config() { ESP_LOGCONFIG(TAG, "BLE keyboard is connected: %s", YESNO(isConnected())); }

#ifdef USE_BINARY_SENSOR
void BLEKeyboard::set_connected_binary_sensor(binary_sensor::BinarySensor *sensor) { connected_ = sensor; }
#endif

hid::Keyboard *BLEKeyboard::keyboard_control() { return &kbd_report_; }
hid::MediaKeys *BLEKeyboard::media_keys_control() { return &media_report_; }

void BLEKeyboard::onWrite(BLECharacteristic *me) {
  uint8_t *led = (uint8_t *) (me->getValue().c_str());
  kbd_report_.set_led_indicator(*led);
}

}  // namespace ble_keyboard
}  // namespace esphome
#endif
