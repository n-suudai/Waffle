#pragma once


// Require Modules
#include "application.h"


// Either Modules


// Optional Modules


// Public Interfaces
#include "hid/peripheral_device_manager.h"
#include "hid/keyboard/keyboard.h"
#include "hid/mouse/mouse.h"
#include "hid/game_pad/game_pad.h"


namespace waffle {
namespace hid {


bool initialize();

void finalize();


} // namespace hid
} // namespace waffle

