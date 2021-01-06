#pragma once


#include "modules/hid/entity.h"
#include "memory/stl.h"
#include <chrono>


namespace waffle {
namespace hid {


class IKeyboard;
class IMouse;
class IGamePad;


class IPeripheralDeviceManager : public HIDEntity
{
public:
    virtual ~IPeripheralDeviceManager() = default;


    [[nodiscard]] virtual wfl::size_t keyboardCount() const = 0;

    [[nodiscard]] virtual bool createKeyboardUnique(
        wfl::size_t index, UniquePtr<IKeyboard>& outKeyboard) const = 0;

    [[nodiscard]] virtual bool createKeyboardShared(
        wfl::size_t index, SharedPtr<IKeyboard>& outKeyboard) const = 0;


    [[nodiscard]] virtual wfl::size_t mouseCount() const = 0;

    [[nodiscard]] virtual bool createMouseUnique(
        wfl::size_t index, UniquePtr<IMouse>& outMouse) const = 0;

    [[nodiscard]] virtual bool createMouseShared(
        wfl::size_t index, SharedPtr<IMouse>& outMouse) const = 0;
    

    [[nodiscard]] virtual wfl::size_t gamePadCount() const = 0;

    [[nodiscard]] virtual bool createGamePadUnique(
        wfl::size_t index, UniquePtr<IGamePad>& outGamePad) const = 0;

    [[nodiscard]] virtual bool createGamePadShared(
        wfl::size_t index, SharedPtr<IGamePad>& outGamePad) const = 0;
};


struct InitializeParameters
{
    WFL_NO_NEW_DELETE;

    void* windowHandle;
    void* applicationHandle;
};


[[nodiscard]] bool createPeripheralDeviceManagerUnique(
    const InitializeParameters& initializeParameters,
    UniquePtr<IPeripheralDeviceManager>& outPeripheralDeviceManager);

[[nodiscard]] bool createPeripheralDeviceManagerShared(
    const InitializeParameters& initializeParameters,
    SharedPtr<IPeripheralDeviceManager>& outPeripheralDeviceManager);


} // namespace hid
} // namespace waffle

