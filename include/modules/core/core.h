#pragma once


// System
#include "system/module_entry/module_entry.h"


// Require Modules


// Either Modules


// Optional Modules


// Public Interfaces
#include "modules/core/logging/logging.h"


namespace waffle {
namespace core {


[[nodiscard]] String moduleName();

[[nodiscard]] bool moduleEntry(
    SharedPtr<modules::Entry>& moduleEntry,
    modules::RuntimeModules& runtimeModules);


} // namespace core
} // namespace waffle

