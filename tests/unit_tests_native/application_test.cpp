
#include <gtest/gtest.h>
#include "mocks/mock_switch.h"
#include "mocks/mock_sensor.h"
#include "mocks/mock_custom_entity.h"
#include "esphome/core/application.h"

#include <cxxabi.h>

namespace esphome {
Application App;  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
namespace test {

class ApplicationTest : public ::testing::Test {
 protected:
  void SetUp() override {  // NOLINT
    // ASSERT_EQ(App.get_entities().size(), 0);
    register_entity_<Switch>(sw_1_);
    register_entity_<Switch>(sw_2_, true);
    register_entity_<Switch>(sw_3_);
    register_entity_<Sensor>(sensor_1_);
    register_entity_<Sensor>(sensor_2_, true);
    register_entity_<Sensor>(sensor_3_, true);
    // ASSERT_EQ(App.get_entities().size(), std::max(SWITCH, SENSOR) + 1);
  }

  template<typename Entity, typename T> void register_entity_(T &t, bool internal = false) {
    t.set_internal(internal);
    t.set_name(std::to_string(id_++));
    App.register_entity(static_cast<Entity *>(&t));
  }

  template<typename T> static void clear(const T &arg) {
    const_cast<T &>(arg).clear();
    ASSERT_EQ(arg.size(), 0);
  }

  template<typename Tuple, size_t... Indices> void clear_impl(const Tuple &tuple, index_sequence<Indices...>) {
    int dummy[] = {0, ((void) clear(std::get<Indices>(tuple)), 0)...};
    (void) dummy;
  }

  template<typename... Args> void clear(const std::tuple<Args...> &tuple) {
    clear_impl(tuple, make_index_sequence<sizeof...(Args)>{});
  }

  void TearDown() override { clear(App.get_entities()); }

  MockSwitch sw_1_;
  MockSwitch sw_2_;
  MockSwitch sw_3_;
  MockSensor sensor_1_;
  MockSensor sensor_2_;
  MockSensor sensor_3_;
  int id_{0};
};

TEST_F(ApplicationTest, GetEntity) {  // NOLINT
  EXPECT_EQ(App.get_entities<Sensor>().size(), 3);
  EXPECT_EQ(App.get_entities<Switch>().size(), 3);
  MockCustomEntity ce_1;
  register_entity_<CustomEntity>(ce_1);
  EXPECT_EQ(App.get_entities<CustomEntity>().size(), 1);
  MockCustomEntity ce_2;
  register_entity_<CustomEntity>(ce_2);
  EXPECT_EQ(App.get_entities<CustomEntity>().size(), 2);
}

TEST_F(ApplicationTest, GetEntityByKey) {  // NOLINT
  EXPECT_NE(App.get_entity_by_key<Switch>(sw_1_.get_object_id_hash()), nullptr);
  EXPECT_EQ(App.get_entity_by_key<Sensor>(sw_1_.get_object_id_hash()), nullptr);
  EXPECT_EQ(App.get_entity_by_key<Switch>(sw_2_.get_object_id_hash()), nullptr);
  EXPECT_NE(App.get_entity_by_key<Switch>(sw_2_.get_object_id_hash(), true), nullptr);
}

}  // namespace test
}  // namespace esphome