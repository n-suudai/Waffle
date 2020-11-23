#include "core/logging.h"
#include "common/platform.h"


namespace waffle {
namespace logging {


void put(const char* text)
{
    ::OutputDebugStringA(text);
    ::OutputDebugStringA("\n");
}


} // namespace logging
} // namespace waffle

