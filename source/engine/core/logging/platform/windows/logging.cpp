#include "core/logging/logging.h"
#include "common/platform/platform.h"


namespace Engine {
namespace Core {


EVoid OutputLog(const EChar* text)
{
    ::OutputDebugStringA(text);
    ::OutputDebugStringA("\n");
}


} // namespace Core
} // namespace Engine

