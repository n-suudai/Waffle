
#include "dinput_peripheral_device_manager.h"


namespace waffle {
namespace hid {


UniquePtr<IPeripheralDeviceManager> DInputPeripheralDeviceManager::createUnique(
    const InitializeParameters& initializeParameters)
{
    return WFL_MAKE_UNIQUE(DInputPeripheralDeviceManager, initializeParameters);
}

SharedPtr<IPeripheralDeviceManager> DInputPeripheralDeviceManager::createShared(
    const InitializeParameters& initializeParameters)
{
    return WFL_MAKE_SHARED(DInputPeripheralDeviceManager, initializeParameters);
}


DInputPeripheralDeviceManager::DInputPeripheralDeviceManager(
    const InitializeParameters& initializeParameters)
    : m_hWindow(reinterpret_cast<HWND>(initializeParameters.windowHandle))
    , m_hInstance(reinterpret_cast<HINSTANCE>(initializeParameters.applicationHandle))
    , m_directInput()
    , m_keyboardDevices()
    , m_mouseDevices()
    , m_gamePadDevices()
{
    if (!m_hWindow || !m_hInstance) { return; }

    HRESULT hr = ::DirectInput8Create(
        m_hInstance,
        DIRECTINPUT_VERSION,
        IID_IDirectInput8A,
        &m_directInput,
        NULL);
    if (FAILED(hr))
    {
        assert(false);
        return;
    }

    // Keyboard
    hr = m_directInput->EnumDevices(
        DI8DEVCLASS_KEYBOARD,
        &DInputPeripheralDeviceManager::EnumAndCreateKeyboard,
        this,
        DIEDFL_ATTACHEDONLY);
    if (FAILED(hr))
    {
        assert(false);
        return;
    }

    // Mouse
    hr = m_directInput->EnumDevices(
        DI8DEVCLASS_POINTER,
        &DInputPeripheralDeviceManager::EnumAndCreateMouse,
        this,
        DIEDFL_ATTACHEDONLY);
    if (FAILED(hr))
    {
        assert(false);
        return;
    }

    // GamePad
    hr = m_directInput->EnumDevices(
        DI8DEVCLASS_GAMECTRL,
        &DInputPeripheralDeviceManager::EnumAndCreateGamePad,
        this,
        DIEDFL_ATTACHEDONLY);
    if (FAILED(hr))
    {
        assert(false);
        return;
    }
}

DInputPeripheralDeviceManager::~DInputPeripheralDeviceManager()
{}

void DInputPeripheralDeviceManager::updateAll(wfl::chrono::microseconds deltaTime)
{
    deltaTime;
}

wfl::size_t DInputPeripheralDeviceManager::keyboardCount() const
{
    return m_keyboardDevices.size();
}

wfl::size_t DInputPeripheralDeviceManager::mouseCount() const
{
    return m_mouseDevices.size();
}

wfl::size_t DInputPeripheralDeviceManager::gamePadCount() const
{
    return m_gamePadDevices.size();
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


} // namespace hid
} // namespace waffle

