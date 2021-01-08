#pragma once


#include "modules/hid/hid.h"


namespace waffle {
namespace hid {


class GameInput : public HIDEntity
{
public:
    GameInput();

    ~GameInput();

    bool initialize(const UniquePtr<application::IWindow>& window);

    bool finalize();

    void update(const Duration& deltaTime);

    bool isAnyPressed() const;

private:
    UniquePtr<IPeripheralDeviceManager> m_peripheralDeviceManager;
    UniquePtr<IKeyboard> m_keyboard;
    UniquePtr<IMouse> m_mouse;
    UniquePtr<IGamePad> m_gamePad;
};


} // namespace hid
} // namespace waffle

