#pragma once

#include "entity.h"
#include "memory/stl.h"
#include "hid.h"


namespace waffle {
namespace application {

class IWindow;

} // namespace application


class GameInput : public RuntimeEntity
{
public:
    GameInput();

    ~GameInput();

    bool initialize(const UniquePtr<application::IWindow>& window);

    void update(const hid::Duration& deltaTime);

    bool isAnyPressed() const;

private:
    UniquePtr<hid::IPeripheralDeviceManager> m_peripheralDeviceManager;
    UniquePtr<hid::IKeyboard> m_keyboard;
    UniquePtr<hid::IMouse> m_mouse;
    UniquePtr<hid::IGamePad> m_gamePad;
};


} // namespace waffle

