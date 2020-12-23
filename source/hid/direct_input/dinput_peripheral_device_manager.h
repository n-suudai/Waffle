#pragma once


#include "hid/peripheral_device_manager.h"
#include "common/platform/win32.h"
#include "direct_input.h"


namespace waffle {
namespace hid {


class IKeyboard;
class IMouse;
class IGamePad;
class DInputPeripheralDeviceManager final : public IPeripheralDeviceManager
{
public:
    static UniquePtr<IPeripheralDeviceManager> createUnique(
        const InitializeParameters& initializeParameters);

    static SharedPtr<IPeripheralDeviceManager> createShared(
        const InitializeParameters& initializeParameters);

public:
    DInputPeripheralDeviceManager(const InitializeParameters& initializeParameters);

    ~DInputPeripheralDeviceManager();

    void updateAll(wfl::chrono::microseconds deltaTime) override;

    wfl::size_t keyboardCount() const override;

    wfl::size_t mouseCount() const override;

    wfl::size_t gamePadCount() const override;

private:
    static BOOL CALLBACK EnumAndCreateKeyboard(LPCDIDEVICEINSTANCEA lpddi, LPVOID pvRef);

    static BOOL CALLBACK EnumAndCreateMouse(LPCDIDEVICEINSTANCEA lpddi, LPVOID pvRef);

    static BOOL CALLBACK EnumAndCreateGamePad(LPCDIDEVICEINSTANCEA lpddi, LPVOID pvRef);

    static BOOL CALLBACK EnumAndSettingAxesCallback(LPCDIDEVICEOBJECTINSTANCEA lpddoi, LPVOID pvRef);

private:
    HWND m_hWindow;;
    HINSTANCE m_hInstance;
    ComPtr<IDirectInput8A> m_directInput;
    Vector<ComPtr<IDirectInputDevice8A>> m_keyboardDevices;
    Vector<ComPtr<IDirectInputDevice8A>> m_mouseDevices;
    Vector<ComPtr<IDirectInputDevice8A>> m_gamePadDevices;
};


} // namespace hid
} // namespace waffle

