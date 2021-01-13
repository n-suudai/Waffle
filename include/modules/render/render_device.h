#pragma once


#include "modules/render/entity.h"


namespace waffle {
namespace render {


class IRenderDevice : public RenderEntity
{
public:
    virtual ~IRenderDevice() = default;

    [[nodiscard]] virtual bool clear() = 0;
};


struct InitializeParameters
{
    WFL_NO_NEW_DELETE;

    void* windowHandle;
};


[[nodiscard]] bool createRenderDeviceUnique(
    const InitializeParameters& initializeParameters,
    UniquePtr<IRenderDevice>& outRenderDevice);

[[nodiscard]] bool createRenderDeviceShared(
    const InitializeParameters& initializeParameters,
    SharedPtr<IRenderDevice>& outRenderDevice);


} // namespace render
} // namespace waffle