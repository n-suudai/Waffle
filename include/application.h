#pragma once


#include "common/types.h"
#include "application/allocation.h"


namespace waffle {
namespace application {


bool initialize(Allocation allocation);

void finalize();


} // namespace application
} // namespace waffle

