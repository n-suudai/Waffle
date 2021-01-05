#pragma once


#include "modules/hid/peripheral_device_manager.h"
#include "direct_input.h"


namespace waffle {
namespace hid {


class DInputPeripheralDeviceManager final : public IPeripheralDeviceManager
{
public:
    static bool createUnique(
        const InitializeParameters& initializeParameters,
        UniquePtr<IPeripheralDeviceManager>& outPeripheralDeviceManager);

    static bool createShared(
        const InitializeParameters& initializeParameters,
        SharedPtr<IPeripheralDeviceManager>& outPeripheralDeviceManager);

public:
    DInputPeripheralDeviceManager();

    ~DInputPeripheralDeviceManager();

    wfl::size_t keyboardCount() const override;

    bool createKeyboardUnique(
        wfl::size_t index, UniquePtr<IKeyboard>& outKeyboard) const override;

    bool createKeyboardShared(
        wfl::size_t index, SharedPtr<IKeyboard>& outKeyboard) const override;


    wfl::size_t mouseCount() const override;

    bool createMouseUnique(
        wfl::size_t index, UniquePtr<IMouse>& outMouse) const override;

    bool createMouseShared(
        wfl::size_t index, SharedPtr<IMouse>& outMouse) const override;


    wfl::size_t gamePadCount() const override;

    bool createGamePadUnique(
        wfl::size_t index, UniquePtr<IGamePad>& outGamePad) const override;

    bool createGamePadShared(
        wfl::size_t index, SharedPtr<IGamePad>& outGamePad) const override;

private:
    static BOOL CALLBACK EnumAndCreateKeyboard(LPCDIDEVICEINSTANCEA lpddi, LPVOID pvRef);

    static BOOL CALLBACK EnumAndCreateMouse(LPCDIDEVICEINSTANCEA lpddi, LPVOID pvRef);

    static BOOL CALLBACK EnumAndCreateGamePad(LPCDIDEVICEINSTANCEA lpddi, LPVOID pvRef);

    static BOOL CALLBACK EnumAndSettingAxesCallback(LPCDIDEVICEOBJECTINSTANCEA lpddoi, LPVOID pvRef);

private:
    bool initialize(const InitializeParameters& initializeParameters);

    bool initializeDirectInput();

    bool initializeKeyboardDevices();

    bool initializeMouseDevices();

    bool initializeGamePadDevices();

    bool initializePreferredJoyConfig();

private:
    HWND m_hWindow;;
    HINSTANCE m_hInstance;
    ComPtr<IDirectInput8A> m_directInput;
    Vector<ComPtr<IDirectInputDevice8A>> m_keyboardDevices;
    Vector<ComPtr<IDirectInputDevice8A>> m_mouseDevices;
    Vector<ComPtr<IDirectInputDevice8A>> m_gamePadDevices;
    DIJOYCONFIG m_preferredJoyConfig;
    bool m_preferredJoyConfigValid;
};


} // namespace hid
} // namespace waffle

