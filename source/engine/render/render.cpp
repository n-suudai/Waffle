#include "render/render.h"
#include "core/logging/logging.h"


namespace Engine {
namespace Render {


EBool Initialize()
{
    Core::OutputLog("[Render] Initialize");
    return true;
}

EVoid Finalize()
{
    Core::OutputLog("[Render] Finalize");
}


} // namespace Render
} // namespace Engine

