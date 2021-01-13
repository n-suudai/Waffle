#include "modules/render/render.h"


namespace waffle {
namespace render {


bool initialize()
{
    logging::put("[render] initialized.");

    return true;
}

void finalize()
{
    logging::put("[render] finalized.");
}


class RenderEntry : public modules::Entry
{
public:
    [[nodiscard]] static bool createShared(
        SharedPtr<modules::Entry>& moduleEntry,
        modules::RuntimeModules& runtimeModules)
    {
        SharedPtr<RenderEntry> ptr = WFL_MAKE_SHARED(RenderEntry, runtimeModules);
        if (ptr)
        {
            moduleEntry = wfl::move(ptr);
            return true;
        }
        return false;
    }

    RenderEntry(modules::RuntimeModules& modules)
        : Entry(modules)
    {}

    bool entry() override
    {
        using namespace modules;

        if (!moduleEntry(EntryPoint::Initialize, wfl::bind(&RenderEntry::initialize, this)))
        {
            return false;
        }

        if (!moduleEntry(EntryPoint::Setup, wfl::bind(&RenderEntry::setup, this)))
        {
            return false;
        }

        if (!moduleEntry(EntryPoint::WaitRenderring, wfl::bind(&RenderEntry::waitRenderring, this)))
        {
            return false;
        }

        if (!moduleEntry(EntryPoint::BeginRenderring, wfl::bind(&RenderEntry::beginRenderring, this)))
        {
            return false;
        }

        if (!moduleEntry(EntryPoint::Terminate, wfl::bind(&RenderEntry::terminate, this)))
        {
            return false;
        }

        if (!moduleEntry(EntryPoint::Finalize, wfl::bind(&RenderEntry::finalize, this)))
        {
            return false;
        }

        return true;
    }

    String moduleName() const override
    {
        return "render";
    }

    [[nodiscard]] const void* getProperty(const String&) const override
    {
        return nullptr;
    }

private:
    bool initialize()
    {
        return render::initialize();
    }

    bool setup()
    {
        InitializeParameters initializeParameters = {};

        WeakPtr<Entry> weakApplication;
        if (!m_modules.getModule("application", weakApplication))
        {
            return false;
        }

        SharedPtr<Entry> sharedApplication = weakApplication.lock();

        if (!sharedApplication) { return false; }

        const UniquePtr<application::IWindow>* window =
            reinterpret_cast<const UniquePtr<application::IWindow>*>(sharedApplication->getProperty("window"));

        if (!window) { return false; }

        initializeParameters.windowHandle = (*window)->windowHandle();

        return createRenderDeviceUnique(
            initializeParameters,
            m_renderDevice);
    }

    bool waitRenderring()
    {
        return true;
    }

    bool beginRenderring()
    {
        if (!m_renderDevice->clear()) { return false; }

        return true;
    }

    bool terminate()
    {
        m_renderDevice.reset();

        return true;
    }

    bool finalize()
    {
        render::finalize();

        return true;
    }

private:
    UniquePtr<IRenderDevice> m_renderDevice;
};


bool moduleEntry(
    SharedPtr<modules::Entry>& moduleEntry,
    modules::RuntimeModules& runtimeModules)
{
    return RenderEntry::createShared(
        moduleEntry,
        runtimeModules);
}


} // namespace render
} // namespace waffle



