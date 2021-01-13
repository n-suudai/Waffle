#pragma once


#include "modules/render/render_device.h"
#include "system/common/platform/win32.h"
#include "system/common/utility/rectangle.h"
#include <d3d11.h>


namespace waffle {
namespace render {


class D3D11RenderDevice final : public IRenderDevice
{
public:
    [[nodiscard]] static bool createUnique(
        const InitializeParameters& initializeParameters,
        UniquePtr<IRenderDevice>& outRenderDevice);

    [[nodiscard]] static bool createShared(
        const InitializeParameters& initializeParameters,
        SharedPtr<IRenderDevice>& outRenderDevice);

public:
    D3D11RenderDevice();

    ~D3D11RenderDevice();

    [[nodiscard]] bool clear() override;

private:
    [[nodiscard]] bool initialize(const InitializeParameters& initializeParameters);


    [[nodiscard]] bool createBackBuffer(const Rectangle<wfl::int32_t>& clientRect);

private:
    HWND m_hWindow;
    Rectangle<wfl::int32_t> m_clientRect;
    UINT m_backBufferCount;

    DXGI_FORMAT m_backBufferFormat;
    D3D_FEATURE_LEVEL m_featureLevel;
    DXGI_SAMPLE_DESC m_sampleDesc;

    ComPtr<IDXGIFactory> m_factory;
    ComPtr<IDXGISwapChain> m_swapChain;

    ComPtr<ID3D11Device> m_device;
    ComPtr<ID3D11DeviceContext> m_immediateContext;
    ComPtr<ID3D11RenderTargetView> m_renderTargetView;
};


} // namespace render
} // namespace waffle

