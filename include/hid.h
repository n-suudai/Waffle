#pragma once


#include "hid/peripheral_device_manager.h"
#include "hid/keyboard.h"
#include "hid/mouse.h"
#include "hid/game_pad.h"


namespace waffle {
namespace hid {


bool initialize();

void finalize();


} // namespace hid
} // namespace waffle

