#pragma once


#include "hid/key_code.h"
#include "hid/peripheral_device_manager.h"


namespace waffle {
namespace hid {


bool initialize();

void finalize();


} // namespace hid
} // namespace waffle

