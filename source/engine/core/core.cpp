#include "core.h"
#include "logging/logging.h"

namespace Engine {
namespace Core {


EBool Initialize()
{
    OutputLog("[Core] Initialize");
    return true;
}

EVoid Finalize()
{
    OutputLog("[Core] Finalize");
}


} // namespace Core
} // namespace Engine
