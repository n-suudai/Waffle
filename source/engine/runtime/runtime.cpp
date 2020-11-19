
#include "core/core.h"
#include "render/render.h"
#include "sound/sound.h"
#include "hid/hid.h"


namespace Engine {
namespace Runtime {


EInt32 RuntimeMain()
{
    Core::Initialize();
    Render::Initialize();
    Sound::Initialize();
    HID::Initialize();


    HID::Finalize();
    Sound::Finalize();
    Render::Finalize();
    Core::Finalize();

    return 0;
}


} // namespace Runtime
} // namespace Engine

