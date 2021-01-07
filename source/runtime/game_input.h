#pragma once

#include "entity.h"
#include "system/memory/stl/stl.h"
#include "modules/hid/hid.h"


namespace waffle {


class GameInput : public RuntimeEntity
{
public:
    GameInput();

    ~GameInput();

    bool initialize(const UniquePtr<application::IWindow>& window);

    bool finalize();

    void update(const hid::Duration& deltaTime);

    bool isAnyPressed() const;

private:
    UniquePtr<hid::IPeripheralDeviceManager> m_peripheralDeviceManager;
    UniquePtr<hid::IKeyboard> m_keyboard;
    UniquePtr<hid::IMouse> m_mouse;
    UniquePtr<hid::IGamePad> m_gamePad;
};


} // namespace waffle

