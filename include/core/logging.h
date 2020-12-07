#pragma once

#include "common/types.h"


namespace waffle {
namespace logging {


void put(const char* text);
void format(const char* str, ...);


} // namespace logging
} // namespace waffle

