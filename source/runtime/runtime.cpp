
#include "runtime_heap.h"
#include "core/logging.h"
#include "memory.h"
#include "core.h"
#include "application.h"
#include "hid.h"



namespace waffle {
namespace runtime {


wfl::int32_t runtimeMain()
{
    memory::initialize();
    core::initialize();
    application::initialize();
    hid::initialize();

    logging::format("Hello! %s!", "Waffle");

    {
        constexpr Rectangle<wfl::int32_t> defaultClientRect(0, 640, 0, 480);
        constexpr Rectangle<wfl::int32_t> customClientRect(200, 640, 200, 480);

        UniquePtr<application::IWindow> window;
        bool result = application::createWindowUnique(defaultClientRect, window);
        if (!result) { return 0; }

        window->setTitle("Waffle");
        window->setClientRect(customClientRect);

        hid::InitializeParameters initializeParameters = {};
        initializeParameters.windowHandle = window->windowHandle();
        initializeParameters.applicationHandle = window->applicationHandle();

        UniquePtr<hid::IPeripheralDeviceManager> manager;
        result = hid::createPeripheralDeviceManagerUnique(
            initializeParameters,
            manager);

        if (!result) { return 0; }

        SharedPtr<hid::IKeyboard> keyboard;
        result = manager->createKeyboardShared(0, keyboard);

        if (!result) { return 0; }

        // loop
        while (window->isAlive())
        {
            keyboard->update(hid::Duration(1));

            if (keyboard->isFirstPressed((hid::KeyCode)hid::KeyCodeType::Key_Escape))
            {
                break;
            }

            if (!window->messagePump())
            {
                break;
            }
        }
    }

    hid::finalize();
    application::finalize();
    core::finalize();
    RuntimeHeap::printDebug_Report_MemoryAll(); // メモリの状態を表示
    memory::finalize();

    return 0;
}


} // namespace runtime
} // namespace waffle

