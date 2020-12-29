﻿
#include "runtime_heap.h"
#include "core/logging.h"
#include "memory.h"
#include "core.h"
#include "application.h"
#include "hid.h"
#include "common/utility/loop_timer.h"



namespace waffle {
namespace runtime {


bool runtimeInitialize()
{
    if (!memory::initialize()) { return false; }
    if (!core::initialize()) { return false; }
    if (!application::initialize()) { return false; }
    if (!hid::initialize()) { return false; }
    return true;
}

void runtimeFinalize()
{
    hid::finalize();
    application::finalize();
    core::finalize();
    RuntimeHeap::printDebug_Report_MemoryAll(); // メモリの状態を表示
    memory::finalize();
}

void runtimeBody()
{
    logging::format("Hello! %s!", "Waffle");

    constexpr Rectangle<wfl::int32_t> defaultClientRect(0, 640, 0, 480);
    constexpr Rectangle<wfl::int32_t> customClientRect(200, 640, 200, 480);

    UniquePtr<application::IWindow> window;
    if (!application::createWindowUnique(defaultClientRect, window)) { return; }
    if (!window->setTitle("Waffle")) { return; }
    if (!window->setClientRect(customClientRect)) { return; }


    hid::InitializeParameters initializeParameters = {};
    initializeParameters.windowHandle = window->windowHandle();
    initializeParameters.applicationHandle = window->applicationHandle();

    UniquePtr<hid::IPeripheralDeviceManager> hidManager;
    if (!hid::createPeripheralDeviceManagerUnique(initializeParameters, hidManager)) { return; }

    SharedPtr<hid::IKeyboard> keyboard;
    if (!hidManager->createKeyboardShared(0, keyboard)) { return; }

    SharedPtr<hid::IMouse> mouse;
    if (!hidManager->createMouseShared(0, mouse)) { return; }

    //SharedPtr<hid::IGamePad> gamePad;
    //if (!hidManager->createGamePadShared(0, gamePad)) { return; }

    LoopTimer loopTimer;

    // loop
    while (window->isAlive())
    {
        hid::Duration elapsed = loopTimer.update<hid::Duration>();

        keyboard->update(elapsed);
        mouse->update(elapsed);
        //gamePad->update(loop_elapsed_duration);

        if (keyboard->isFirstPressed(hid::KeyCodeType::Key_Escape))
        {
            break;
        }

        if (mouse->isFirstPressed(hid::MouseButtonType::Button_0))
        {
            break;
        }

        //if (gamePad->isFirstPressed(hid::POVType::POV_0))
        //{
        //    break;
        //}

        //if (gamePad->analogInputValue(hid::AnalogInputType::RightThumbStickY) > 500)
        //{
        //    break;
        //}

        if (!window->messagePump())
        {
            break;
        }
    }
}


wfl::int32_t runtimeMain()
{
    if (!runtimeInitialize())
    {
        runtimeFinalize();
        return 0;
    }

    runtimeBody();

    runtimeFinalize();

    return 0;
}


} // namespace runtime
} // namespace waffle

