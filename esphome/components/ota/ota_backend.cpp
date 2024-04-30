#include "ota_backend.h"

namespace esphome {
namespace ota {

#ifdef USE_OTA_STATE_CALLBACK
OTAGlobalCallback *global_ota_component = new OTAGlobalCallback;  // NOLINT(cppcoreguidelines-owning-memory)
#endif

}  // namespace ota
}  // namespace esphome
