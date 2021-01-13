#include "d3d11_render_device.h"
#include "modules/render/render.h"
#include "system/common/platform/win32/win32_utility.h"


namespace waffle {
namespace render {


String getResultText(HRESULT hr)
{
    String result = "";

    LPVOID lpMsgBuf = nullptr;

    ::FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        hr,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // 既定の言語
        (LPSTR)&lpMsgBuf,
        0,
        NULL
    );

    // 文字列をコピーする。
    if (lpMsgBuf != nullptr)
    {
        result = (LPCSTR)lpMsgBuf;
    }

    // バッファを解放する。
    ::LocalFree(lpMsgBuf);

    return result;
}

bool outputLogIfFailed(HRESULT hr)
{
    if (SUCCEEDED(hr)) { return true; }

    logging::format("[render]:%s", getResultText(hr).c_str());

    return false;
}

static constexpr UINT getCreateDeviceFlags()
{
    // BGRAサポートを有効化
    UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(DEBUG) || defined(_DEBUG)
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    return createDeviceFlags;
}


bool D3D11RenderDevice::createUnique(
    const InitializeParameters& initializeParameters,
    UniquePtr<IRenderDevice>& outRenderDevice)
{
    UniquePtr<D3D11RenderDevice> ptr =
        WFL_MAKE_UNIQUE(D3D11RenderDevice);

    if (ptr->initialize(initializeParameters))
    {
        outRenderDevice = wfl::move(ptr);
        return true;
    }

    return false;
}

bool D3D11RenderDevice::createShared(
    const InitializeParameters& initializeParameters,
    SharedPtr<IRenderDevice>& outRenderDevice)
{
    SharedPtr<D3D11RenderDevice> ptr =
        WFL_MAKE_UNIQUE(D3D11RenderDevice);

    if (ptr->initialize(initializeParameters))
    {
        outRenderDevice = wfl::move(ptr);
        return true;
    }

    return false;
}

bool D3D11RenderDevice::clear()
{
    if (!m_device) { return false; }

    if (!m_immediateContext) { return false; }

    if (!m_swapChain) { return false; }

    if (!m_renderTargetView) { return false; }

    // 指定色でクリア
    FLOAT clearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red, green, blue, alpha
    m_immediateContext->ClearRenderTargetView(m_renderTargetView.Get(), clearColor);

    // 結果をウインドウに反映
    HRESULT hr = m_swapChain->Present(0, 0);
    return outputLogIfFailed(hr);
}

D3D11RenderDevice::D3D11RenderDevice()
    : m_hWindow(NULL)
    , m_backBufferCount(2)
    , m_backBufferFormat(DXGI_FORMAT_R8G8B8A8_UNORM)
    , m_featureLevel(D3D_FEATURE_LEVEL_11_1)
{}

D3D11RenderDevice::~D3D11RenderDevice()
{}

bool D3D11RenderDevice::initialize(const InitializeParameters& initializeParameters)
{
    if (!initializeParameters.windowHandle) { return false; }

    m_hWindow = reinterpret_cast<HWND>(initializeParameters.windowHandle);

    if (!getClientRect(m_hWindow, m_clientRect)) { return false; }

    HRESULT hr = E_FAIL;

    // ファクトリーを作成
    hr = ::CreateDXGIFactory(IID_PPV_ARGS(&m_factory));

    if (!outputLogIfFailed(hr)) { return false; }

    // デバイス & コンテキストを作成
    D3D_FEATURE_LEVEL featureLevels[] = {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
    };

    hr = ::D3D11CreateDevice(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE, // ハードウェア ドライバー を使用
        nullptr,
        getCreateDeviceFlags(),
        featureLevels,
        _countof(featureLevels),
        D3D11_SDK_VERSION,
        &m_device,
        &m_featureLevel,
        &m_immediateContext);

    if (!outputLogIfFailed(hr)) { return false; }

    // スワップチェインを作成
    {
        // 使用可能なMSAAを取得
        ZeroMemory(&m_sampleDesc, sizeof(m_sampleDesc));
        for (int i = 1; i <= D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT; i <<= 1)
        {
            UINT Quality;
            if (SUCCEEDED(m_device->CheckMultisampleQualityLevels(DXGI_FORMAT_D24_UNORM_S8_UINT, i, &Quality)))
            {
                if (0 < Quality)
                {
                    m_sampleDesc.Count = i;
                    m_sampleDesc.Quality = Quality - 1;
                }
            }
        }

        // スワップチェインを作成
        DXGI_SWAP_CHAIN_DESC swapChainDesc;
        ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
        swapChainDesc.BufferDesc.Width = m_clientRect.width<UINT>();
        swapChainDesc.BufferDesc.Height = m_clientRect.height<UINT>();
        swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
        swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
        swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_CENTERED;
        swapChainDesc.BufferDesc.Format = m_backBufferFormat;
        swapChainDesc.SampleDesc = m_sampleDesc;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
        swapChainDesc.BufferCount = m_backBufferCount;
        swapChainDesc.Windowed = TRUE;
        swapChainDesc.OutputWindow = m_hWindow;
        swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

        hr = m_factory->CreateSwapChain(
            m_device.Get(),
            &swapChainDesc,
            &m_swapChain);

        if (!outputLogIfFailed(hr)) { return false; }
    }


    return createBackBuffer(m_clientRect);
}

bool D3D11RenderDevice::createBackBuffer(const Rectangle<wfl::int32_t>& clientRect)
{
    if (!m_immediateContext) { return false; }

    if (!m_swapChain) { return false; }

    if (!m_device) { return false; }

    // バックバッファを破棄
    m_immediateContext->OMSetRenderTargets(0, nullptr, nullptr);
    m_renderTargetView.Reset();

    HRESULT hr = E_FAIL;

    // バッファのサイズを変更
    hr = m_swapChain->ResizeBuffers(
        m_backBufferCount,
        clientRect.width<UINT>(),
        clientRect.height<UINT>(),
        m_backBufferFormat,
        DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);

    if (!outputLogIfFailed(hr)) { return false; }

    // レンダーターゲットを生成
    {
        ComPtr<ID3D11Texture2D> backBuffer;

        hr = m_swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));

        if (!outputLogIfFailed(hr)) { return false; }

        hr = m_device->CreateRenderTargetView(
            backBuffer.Get(),
            nullptr,
            &m_renderTargetView);

        if (!outputLogIfFailed(hr)) { return false; }
    }

    // レンダーターゲットを設定
    ID3D11RenderTargetView* pRenderTargetViews[] = {
        m_renderTargetView.Get()
    };
    m_immediateContext->OMSetRenderTargets(_countof(pRenderTargetViews), pRenderTargetViews, nullptr);

    // ビューポートを設定
    D3D11_VIEWPORT viewport;
    viewport.Width = clientRect.width<FLOAT>();
    viewport.Height = clientRect.height<FLOAT>();
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;
    m_immediateContext->RSSetViewports(1, &viewport);

    return true;
}


} // namespace render
} // namespace waffle

