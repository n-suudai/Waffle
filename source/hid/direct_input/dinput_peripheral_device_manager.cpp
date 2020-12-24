﻿
#include "dinput_peripheral_device_manager.h"
#include "dinput_keyboard.h"


namespace waffle {
namespace hid {


bool DInputPeripheralDeviceManager::createUnique(
    const InitializeParameters& initializeParameters,
    UniquePtr<IPeripheralDeviceManager>& outPeripheralDeviceManager)
{
    UniquePtr<DInputPeripheralDeviceManager> ptr =
        WFL_MAKE_UNIQUE(DInputPeripheralDeviceManager);

    if (ptr->initialize(initializeParameters))
    {
        outPeripheralDeviceManager = wfl::move(ptr);
        return true;
    }

    return false;
}

bool DInputPeripheralDeviceManager::createShared(
    const InitializeParameters& initializeParameters,
    SharedPtr<IPeripheralDeviceManager>& outPeripheralDeviceManager)
{
    SharedPtr<DInputPeripheralDeviceManager> ptr =
        WFL_MAKE_SHARED(DInputPeripheralDeviceManager);

    if (ptr->initialize(initializeParameters))
    {
        outPeripheralDeviceManager = wfl::move(ptr);
        return true;
    }

    return false;
}

DInputPeripheralDeviceManager::DInputPeripheralDeviceManager()
    : m_hWindow(NULL)
    , m_hInstance(NULL)
    , m_directInput()
    , m_keyboardDevices()
    , m_mouseDevices()
    , m_gamePadDevices()
{}

DInputPeripheralDeviceManager::~DInputPeripheralDeviceManager()
{}

wfl::size_t DInputPeripheralDeviceManager::keyboardCount() const
{
    return m_keyboardDevices.size();
}

bool DInputPeripheralDeviceManager::createKeyboardUnique(
    wfl::size_t index, UniquePtr<IKeyboard>& outKeyboard) const
{
    if (index >= m_keyboardDevices.size()) { return false; }

    bool result = DInputKeyboard::createUnique(
        m_keyboardDevices[index],
        m_hWindow,
        outKeyboard);

    return result;
}

bool DInputPeripheralDeviceManager::createKeyboardShared(
    wfl::size_t index, SharedPtr<IKeyboard>& outKeyboard) const
{
    if (index >= m_keyboardDevices.size()) { return false; }

    bool result = DInputKeyboard::createShared(
        m_keyboardDevices[index],
        m_hWindow,
        outKeyboard);

    return result;
}

wfl::size_t DInputPeripheralDeviceManager::mouseCount() const
{
    return m_mouseDevices.size();
}

bool DInputPeripheralDeviceManager::createMouseUnique(
    wfl::size_t index, UniquePtr<IMouse>& outMouse) const
{
    if (index >= m_mouseDevices.size()) { return false; }
    
    outMouse;

    return false;
}

bool DInputPeripheralDeviceManager::createMouseShared(
    wfl::size_t index, SharedPtr<IMouse>& outMouse) const
{
    if (index >= m_mouseDevices.size()) { return false; }

    outMouse;

    return false;
}

wfl::size_t DInputPeripheralDeviceManager::gamePadCount() const
{
    return m_gamePadDevices.size();
}

bool DInputPeripheralDeviceManager::createGamePadUnique(
    wfl::size_t index, UniquePtr<IGamePad>& outGamePad) const
{
    if (index >= m_gamePadDevices.size()) { return false; }

    outGamePad;

    return false;
}

bool DInputPeripheralDeviceManager::createGamePadShared(
    wfl::size_t index, SharedPtr<IGamePad>& outGamePad) const
{
    if (index >= m_gamePadDevices.size()) { return false; }

    outGamePad;

    return false;
}


BOOL CALLBACK DInputPeripheralDeviceManager::EnumAndCreateKeyboard(
    LPCDIDEVICEINSTANCEA lpddi, LPVOID pvRef)
{
    DInputPeripheralDeviceManager* self = reinterpret_cast<DInputPeripheralDeviceManager*>(pvRef);
    ComPtr<IDirectInput8A> directInput = self->m_directInput;
    HWND hWindow = self->m_hWindow;

    ComPtr<IDirectInputDevice8A> device;

    HRESULT hr = directInput->CreateDevice(lpddi->guidInstance, &device, nullptr);
    if (FAILED(hr)) { return DIENUM_CONTINUE; }

    hr = device->SetDataFormat(&c_dfDIKeyboard);
    if (FAILED(hr)) { return DIENUM_CONTINUE; }

    hr = device->SetCooperativeLevel(hWindow, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
    if (FAILED(hr)) { return DIENUM_CONTINUE; }

    DIPROPDWORD dipdw = { 0 };
    dipdw.diph.dwSize = sizeof(DIPROPDWORD);
    dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    dipdw.diph.dwObj = 0;
    dipdw.diph.dwHow = DIPH_DEVICE;
    dipdw.dwData = 10;

    hr = device->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);
    if (FAILED(hr)) { return DIENUM_CONTINUE; }

    self->m_keyboardDevices.push_back(device);

    return DIENUM_CONTINUE;
}

BOOL CALLBACK DInputPeripheralDeviceManager::EnumAndCreateMouse(
    LPCDIDEVICEINSTANCEA lpddi, LPVOID pvRef)
{
    DInputPeripheralDeviceManager* self = reinterpret_cast<DInputPeripheralDeviceManager*>(pvRef);
    ComPtr<IDirectInput8A> directInput = self->m_directInput;
    HWND hWindow = self->m_hWindow;

    ComPtr<IDirectInputDevice8A> device;

    HRESULT hr = directInput->CreateDevice(lpddi->guidInstance, &device, nullptr);
    if (FAILED(hr)) { return DIENUM_CONTINUE; }

    hr = device->SetDataFormat(&c_dfDIMouse2);
    if (FAILED(hr)) { return DIENUM_CONTINUE; }

    hr = device->SetCooperativeLevel(hWindow, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
    if (FAILED(hr)) { return DIENUM_CONTINUE; }

    //	自動センタリング無効
    DIPROPDWORD dipdw = { 0 };
    dipdw.diph.dwSize = sizeof(DIPROPDWORD);
    dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    dipdw.diph.dwObj = 0;
    dipdw.diph.dwHow = DIPH_DEVICE;
    dipdw.dwData = DIPROPAUTOCENTER_OFF;

    hr = device->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);
    if (FAILED(hr)) { return DIENUM_CONTINUE; }

    self->m_mouseDevices.push_back(device);

    return DIENUM_CONTINUE;
}

BOOL CALLBACK DInputPeripheralDeviceManager::EnumAndCreateGamePad(
    LPCDIDEVICEINSTANCEA lpddi, LPVOID pvRef)
{
    DInputPeripheralDeviceManager* self = reinterpret_cast<DInputPeripheralDeviceManager*>(pvRef);
    ComPtr<IDirectInput8A> directInput = self->m_directInput;
    HWND hWindow = self->m_hWindow;

    ComPtr<IDirectInputDevice8A> device;

    HRESULT hr = directInput->CreateDevice(lpddi->guidInstance, &device, nullptr);
    if (FAILED(hr)) { return DIENUM_CONTINUE; }

    hr = device->SetDataFormat(&c_dfDIJoystick2);
    if (FAILED(hr)) { return DIENUM_CONTINUE; }

    hr = device->SetCooperativeLevel(hWindow, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
    if (FAILED(hr)) { return DIENUM_CONTINUE; }

    //	自動センタリング無効
    DIPROPDWORD	dipdw = { 0 };
    dipdw.diph.dwSize = sizeof(dipdw);
    dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
    dipdw.diph.dwObj = 0;
    dipdw.diph.dwHow = DIPH_DEVICE;
    dipdw.dwData = DIPROPAUTOCENTER_OFF;

    hr = device->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);
    if (FAILED(hr)) { return DIENUM_CONTINUE; }

    hr = device->EnumObjects(
        &DInputPeripheralDeviceManager::EnumAndSettingAxesCallback,
        reinterpret_cast<LPVOID>(device.Get()),
        DIDFT_AXIS);
    if (FAILED(hr)) { return DIENUM_CONTINUE; }

    self->m_gamePadDevices.push_back(device);

    return DIENUM_CONTINUE;
}

BOOL CALLBACK DInputPeripheralDeviceManager::EnumAndSettingAxesCallback(
    LPCDIDEVICEOBJECTINSTANCEA lpddoi, LPVOID pvRef)
{
    // 軸範囲を設定（-1000～1000）
    DIPROPRANGE diprg = { 0 };

    diprg.diph.dwSize = sizeof(diprg);
    diprg.diph.dwHeaderSize = sizeof(diprg.diph);
    diprg.diph.dwObj = lpddoi->dwType;
    diprg.diph.dwHow = DIPH_BYID;
    diprg.lMin = -1000;
    diprg.lMax = +1000;

    IDirectInputDevice8A* device = reinterpret_cast<IDirectInputDevice8A*>(pvRef);
    HRESULT hr = device->SetProperty(DIPROP_RANGE, &diprg.diph);

    return (FAILED(hr)) ? DIENUM_STOP : DIENUM_CONTINUE;
}

bool DInputPeripheralDeviceManager::initialize(const InitializeParameters& initializeParameters)
{
    DInputKeyboard::buildKeyMapTable();

    m_hWindow = reinterpret_cast<HWND>(initializeParameters.windowHandle);
    m_hInstance = reinterpret_cast<HINSTANCE>(initializeParameters.applicationHandle);

    if (!m_hWindow || !m_hInstance) { return false; }

    if (!initializeDirectInput()) { return false; }

    bool result = initializeKeyboardDevices();

    result |= initializeMouseDevices();

    result |= initializeGamePadDevices();

    return result;
}

bool DInputPeripheralDeviceManager::initializeDirectInput()
{
    // device
    HRESULT hr = ::DirectInput8Create(
        m_hInstance,
        DIRECTINPUT_VERSION,
        IID_IDirectInput8A,
        &m_directInput,
        NULL);

    bool result = SUCCEEDED(hr);

    return result;
}

bool DInputPeripheralDeviceManager::initializeKeyboardDevices()
{
    HRESULT hr = m_directInput->EnumDevices(
        DI8DEVCLASS_KEYBOARD,
        &DInputPeripheralDeviceManager::EnumAndCreateKeyboard,
        this,
        DIEDFL_ATTACHEDONLY);

    bool result = SUCCEEDED(hr);

    return result;
}

bool DInputPeripheralDeviceManager::initializeMouseDevices()
{
    HRESULT hr = m_directInput->EnumDevices(
        DI8DEVCLASS_POINTER,
        &DInputPeripheralDeviceManager::EnumAndCreateMouse,
        this,
        DIEDFL_ATTACHEDONLY);

    bool result = SUCCEEDED(hr);

    return result;
}

bool DInputPeripheralDeviceManager::initializeGamePadDevices()
{
    HRESULT hr = m_directInput->EnumDevices(
        DI8DEVCLASS_GAMECTRL,
        &DInputPeripheralDeviceManager::EnumAndCreateGamePad,
        this,
        DIEDFL_ATTACHEDONLY);

    bool result = SUCCEEDED(hr);

    return result;
}

} // namespace hid
} // namespace waffle

