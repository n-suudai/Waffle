#include "core.h"
#include "memory/memory.h"
#include "logging/logging.h"

namespace Engine {
namespace Core {


EBool Initialize()
{
    OutputLog("[Core] Initialize");

    EBool result = Memory::Initialize();

    return result;
}

EVoid Finalize()
{
    OutputLog("[Core] Finalize");

    Memory::Finalize();
}


} // namespace Core
} // namespace Engine
