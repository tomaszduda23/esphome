#pragma once
#include "gmock/gmock.h"
#include "esphome/core/entity_base.h"

namespace esphome {
namespace test {

struct Switch : public EntityBase {
  virtual void add_on_state_callback(std::function<void(bool)> &&callback) { this->callback_ = callback; }
  void set(bool value) {
    state = value;
    if (callback_) {
      callback_(value);
    }
  }

  bool state{0};

 protected:
  std::function<void(bool)> callback_;
};

struct MockSwitch : public Switch {
  MockSwitch() {
    ON_CALL(*this, add_on_state_callback).WillByDefault([this](std::function<void(bool)> &&callback) {
      return Switch::add_on_state_callback(std::move(callback));
    });
  }
  MOCK_METHOD(void, add_on_state_callback, (std::function<void(bool)> && callback), (override));
};

}  // namespace test
}  // namespace esphome
