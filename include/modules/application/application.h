#pragma once


// Require Modules
#include "modules/core/core.h"


// Either Modules


// Optional Modules


// Public Interfaces
#include "modules/application/window.h"


namespace waffle {
namespace application {


[[nodiscard]] String moduleName();

[[nodiscard]] bool moduleEntry(
    SharedPtr<modules::Entry>& moduleEntry,
    modules::RuntimeModules& runtimeModules);


} // namespace application
} // namespace waffle

