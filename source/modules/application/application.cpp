#include "modules/application/application.h"



namespace waffle {
namespace application {


bool initialize()
{
    logging::put("[application] initialized.");

    return true;
}

void finalize()
{
    logging::put("[application] finalized.");
}


class ApplicationEntry final : public modules::Entry
{
public:
    [[nodiscard]] static bool createShared(
        SharedPtr<modules::Entry>& moduleEntry,
        modules::RuntimeModules& runtimeModules)
    {
        SharedPtr<ApplicationEntry> ptr = WFL_MAKE_SHARED(ApplicationEntry, runtimeModules);
        if (ptr)
        {
            moduleEntry = wfl::move(ptr);
            return true;
        }
        return false;
    }

    ApplicationEntry(modules::RuntimeModules& modules)
        : Entry(modules)
    {}

    bool entry() override
    {
        using namespace modules;

        if (!moduleEntry(EntryPoint::Initialize, wfl::bind(&ApplicationEntry::initialize, this)))
        {
            return false;
        }

        if (!moduleEntry(EntryPoint::Setup, wfl::bind(&ApplicationEntry::setup, this)))
        {
            return false;
        }

        if (!moduleEntry(EntryPoint::Update, wfl::bind(&ApplicationEntry::update, this)))
        {
            return false;
        }

        if (!moduleEntry(EntryPoint::Finalize, wfl::bind(&ApplicationEntry::finalize, this)))
        {
            return false;
        }

        return true;
    }

    String moduleName() const override
    {
        return "application";
    }

    const void* getProperty(const String& name) const override
    {
        if (name == "applicationHandle")
        {
            return m_window ? m_window->applicationHandle() : nullptr;
        }

        if (name == "windowHandle")
        {
            return m_window ? m_window->windowHandle() : nullptr;
        }

        if (name == "window")
        {
            return m_window ? &m_window : nullptr;
        }

        return nullptr;
    }

private:
    bool initialize()
    {
        return application::initialize();
    }

    bool setup()
    {
        constexpr Rectangle<wfl::int32_t> defaultClientRect(0, 640, 0, 480);

        if (!application::createWindowUnique(defaultClientRect, m_window))
        {
            return false;
        }

        if (!m_window->setTitle("Waffle"))
        {
            return false;
        }

        return true;
    }

    bool update()
    {
        if (!m_window) { return false; }

        if (!m_window->isAlive()) { return false; }

        return m_window->messagePump();
    }

    bool finalize()
    {
        m_window.reset();
        application::finalize();
        return true;
    }

private:
    UniquePtr<application::IWindow> m_window;
};


bool moduleEntry(
    SharedPtr<modules::Entry>& moduleEntry,
    modules::RuntimeModules& runtimeModules)
{
    return ApplicationEntry::createShared(
        moduleEntry,
        runtimeModules);
}


} // namespace application
} // namespace waffle
