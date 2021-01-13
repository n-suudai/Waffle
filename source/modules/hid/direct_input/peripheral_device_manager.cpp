
#include "modules/hid/peripheral_device_manager.h"
#include "dinput_peripheral_device_manager.h"


namespace waffle {
namespace hid {


bool createPeripheralDeviceManagerUnique(
    const InitializeParameters& initializeParameters,
    UniquePtr<IPeripheralDeviceManager>& outPeripheralDeviceManager)
{
    return DInputPeripheralDeviceManager::createUnique(
        initializeParameters,
        outPeripheralDeviceManager);
}

bool createPeripheralDeviceManagerShared(
    const InitializeParameters& initializeParameters,
    SharedPtr<IPeripheralDeviceManager>& outPeripheralDeviceManager)
{
    return DInputPeripheralDeviceManager::createShared(
        initializeParameters,
        outPeripheralDeviceManager);
}


} // namespace hid
} // namespace waffle

