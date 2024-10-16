#include "esphome/core/helpers.h"
#include <zephyr/kernel.h>

namespace esphome {
Mutex::Mutex() {
    auto mutex = new k_mutex();
    this->handle_ = mutex;
    k_mutex_init(mutex);
}
Mutex::~Mutex() {
    delete static_cast<k_mutex *>(this->handle_);
}
void Mutex::lock() { k_mutex_lock(static_cast<k_mutex *>(this->handle_), K_FOREVER); }
bool Mutex::try_lock() { return k_mutex_lock(static_cast<k_mutex *>(this->handle_), K_NO_WAIT) == 0; }
void Mutex::unlock() { k_mutex_unlock(static_cast<k_mutex *>(this->handle_)); }
uint32_t random_uint32() { return rand(); }
}