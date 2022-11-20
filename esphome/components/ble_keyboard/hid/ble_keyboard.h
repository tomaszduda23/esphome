#pragma once

#include "esphome/core/defines.h"
#ifdef USE_KEYBOARD
#include "esphome/components/hid/keyboard/hid_keyboard.h"
#include "esphome/components/hid/keyboard/hid_media_keys.h"
#ifdef USE_BINARY_SENSOR
#include "esphome/components/binary_sensor/binary_sensor.h"
#endif
#include "BleKeyboard.h"

namespace esphome {
namespace ble_keyboard {

class BLEKeyboard;

class KeyboardReport : public hid::Keyboard {
 public:
  KeyboardReport(BleKeyboard &kbd) : kbd_(kbd) {}
  void report() override;
  void loop();
  void set_led_indicator(uint8_t led_indicator) {
    led_indicator_ = led_indicator;
    update_led_ = true;
  }

 protected:
  BleKeyboard &kbd_;
  uint8_t led_indicator_{0};
  bool update_led_{false};
};

class MediaKeysReport : public hid::MediaKeys {
 public:
  MediaKeysReport(BleKeyboard &kbd) : kbd_(kbd) {}
  void report() override;

 protected:
  BleKeyboard &kbd_;
};

class BLEKeyboard : public PollingComponent, public BleKeyboard {
 public:
  BLEKeyboard();
  void setup() override;
  void loop() override;
  void update() override;
  void dump_config() override;
#ifdef USE_BINARY_SENSOR
  void set_connected_binary_sensor(binary_sensor::BinarySensor *sensor);
#endif
  hid::Keyboard *keyboard_control();
  hid::MediaKeys *media_keys_control();

 protected:
  KeyboardReport kbd_report_;
  MediaKeysReport media_report_;
#ifdef USE_BINARY_SENSOR
  binary_sensor::BinarySensor *connected_{nullptr};
#endif
  void onWrite(BLECharacteristic *me) override;
};

}  // namespace ble_keyboard
}  // namespace esphome
#endif
