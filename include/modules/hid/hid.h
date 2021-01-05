#pragma once


// Require Modules
#include "modules/application/application.h"


// Either Modules


// Optional Modules


// Public Interfaces
#include "modules/hid/peripheral_device_manager.h"
#include "modules/hid/keyboard/keyboard.h"
#include "modules/hid/mouse/mouse.h"
#include "modules/hid/game_pad/game_pad.h"


namespace waffle {
namespace hid {


bool initialize();

void finalize();


} // namespace hid
} // namespace waffle

