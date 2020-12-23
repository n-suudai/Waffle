
#include "hid/peripheral_device_manager.h"
#include "direct_input/dinput_peripheral_device_manager.h"


namespace waffle {
namespace hid {


UniquePtr<IPeripheralDeviceManager> createPeripheralDeviceManagerUnique(
    const InitializeParameters& initializeParameters)
{
    return DInputPeripheralDeviceManager::createUnique(initializeParameters);
}

SharedPtr<IPeripheralDeviceManager> createPeripheralDeviceManagerShared(
    const InitializeParameters& initializeParameters)
{
    return DInputPeripheralDeviceManager::createShared(initializeParameters);
}


} // namespace hid
} // namespace waffle

