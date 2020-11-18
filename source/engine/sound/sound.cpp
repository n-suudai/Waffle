#include "sound/sound.h"
#include "core/logging/logging.h"


namespace Engine {
namespace Sound {


EBool Initialize()
{
    Core::OutputLog("[Sound] Initialize");
    return true;
}

EVoid Finalize()
{
    Core::OutputLog("[Sound] Finalize");
}


} // namespace Sound
} // namespace Engine

