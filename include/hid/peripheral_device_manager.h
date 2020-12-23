#pragma once


#include "hid/entity.h"
#include "memory/stl.h"
#include <chrono>


namespace waffle {
namespace hid {


class IPeripheralDeviceManager : public HIDEntity
{
public:
    virtual ~IPeripheralDeviceManager() = default;

    virtual void updateAll(wfl::chrono::microseconds deltaTime) = 0;

    virtual wfl::size_t keyboardCount() const = 0;

    virtual wfl::size_t mouseCount() const = 0;
    
    virtual wfl::size_t gamePadCount() const = 0;
};


struct InitializeParameters
{
    WFL_NO_NEW_DELETE;

    void* windowHandle;
    void* applicationHandle;
};


UniquePtr<IPeripheralDeviceManager> createPeripheralDeviceManagerUnique(
    const InitializeParameters& initializeParameters);

SharedPtr<IPeripheralDeviceManager> createPeripheralDeviceManagerShared(
    const InitializeParameters& initializeParameters);


} // namespace hid
} // namespace waffle

