#pragma once


// System
#include "system/module_entry/module_entry.h"


// Require Modules
#include "modules/application/application.h"


// Either Modules


// Optional Modules


// Public Interfaces
#include "modules/render/render_device.h"


namespace waffle {
namespace render {


[[nodiscard]] bool moduleEntry(
    SharedPtr<modules::Entry>& moduleEntry,
    modules::RuntimeModules& runtimeModules);


} // namespace render
} // namespace waffle

