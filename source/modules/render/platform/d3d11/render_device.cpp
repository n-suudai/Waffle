#include "d3d11_render_device.h"


namespace waffle {
namespace render {


bool createRenderDeviceUnique(
    const InitializeParameters& initializeParameters,
    UniquePtr<IRenderDevice>& outRenderDevice)
{
    return D3D11RenderDevice::createUnique(
        initializeParameters,
        outRenderDevice);
}

bool createRenderDeviceShared(
    const InitializeParameters& initializeParameters,
    SharedPtr<IRenderDevice>& outRenderDevice)
{
    return D3D11RenderDevice::createShared(
        initializeParameters,
        outRenderDevice);
}


} // namespace render
} // namespace waffle

