
#include "game_input.h"


namespace waffle {
namespace hid {


GameInput::GameInput()
    : m_peripheralDeviceManager()
    , m_keyboard()
    , m_mouse()
    , m_gamePad()
{}

GameInput::~GameInput() {}

bool GameInput::initialize(const UniquePtr<application::IWindow>& window)
{
    assert(window);

    InitializeParameters initializeParameters = {};
    initializeParameters.windowHandle = window->windowHandle();
    initializeParameters.applicationHandle = window->applicationHandle();

    if (!createPeripheralDeviceManagerUnique(initializeParameters, m_peripheralDeviceManager))
    {
        return false;
    }

    // いずれかが成功ならOK
    bool result = m_peripheralDeviceManager->createKeyboardUnique(0, m_keyboard);

    result |= m_peripheralDeviceManager->createMouseUnique(0, m_mouse);

    result |= m_peripheralDeviceManager->createGamePadUnique(0, m_gamePad);

    return result;
}

bool GameInput::finalize()
{
    m_gamePad.reset();
    m_mouse.reset();
    m_keyboard.reset();
    m_peripheralDeviceManager.reset();

    return true;
}

void GameInput::update(const Duration& deltaTime)
{
    if (!m_peripheralDeviceManager) { return; }

    if (m_keyboard)
    {
        m_keyboard->update(deltaTime);
    }

    if (m_mouse)
    {
        m_mouse->update(deltaTime);
    }

    if (m_gamePad)
    {
        m_gamePad->update(deltaTime);
    }
}

bool GameInput::isAnyPressed() const
{
    bool result = false;

    if (m_keyboard)
    {
        result = m_keyboard->isAnyPressed();
    }

    if (m_mouse && !result)
    {
        result = m_mouse->isAnyPressed();
    }

    if (m_gamePad && !result)
    {
        result = m_gamePad->isAnyPressed();
    }

    return result;
}


} // namespace hid
} // namespace waffle

