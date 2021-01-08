#include "game_input.h"
#include "system/common/utility/loop_timer.h"


namespace waffle {
namespace hid {


bool initialize()
{
    logging::put("[hid] initialized.");

    return true;
}

void finalize()
{
    logging::put("[hid] finalized.");
}

class HIDEntry : public modules::Entry
{
public:
    [[nodiscard]] static bool createShared(
        SharedPtr<modules::Entry>& moduleEntry,
        modules::RuntimeModules& runtimeModules)
    {
        SharedPtr<HIDEntry> ptr = WFL_MAKE_SHARED(HIDEntry, runtimeModules);
        if (ptr)
        {
            moduleEntry = wfl::move(ptr);
            return true;
        }
        return false;
    }

    HIDEntry(modules::RuntimeModules& modules)
        : Entry(modules)
    {}

    bool entry() override
    {
        using namespace modules;

        if (!moduleEntry(EntryPoint::Initialize, wfl::bind(&HIDEntry::initialize, this)))
        {
            return false;
        }

        if (!moduleEntry(EntryPoint::Setup, wfl::bind(&HIDEntry::setup, this)))
        {
            return false;
        }

        if (!moduleEntry(EntryPoint::Update, wfl::bind(&HIDEntry::update, this)))
        {
            return false;
        }

        if (!moduleEntry(EntryPoint::Terminate, wfl::bind(&HIDEntry::terminate, this)))
        {
            return false;
        }

        if (!moduleEntry(EntryPoint::Finalize, wfl::bind(&HIDEntry::finalize, this)))
        {
            return false;
        }

        return true;
    }

    const void* getProperty(const String&) const override
    {
        return nullptr;
    }

private:
    bool initialize()
    {
        return hid::initialize();
    }

    bool setup()
    {
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

        if (!m_gameInput.initialize(*window)) { return false; }

        m_loopTimer = LoopTimer();

        return true;
    }

    bool update()
    {
        hid::Duration elapsed = m_loopTimer.update<hid::Duration>();

        m_gameInput.update(elapsed);

        if (m_gameInput.isAnyPressed()) { return false; }

        return true;
    }

    bool terminate()
    {
        if (!m_gameInput.finalize()) { return false; }

        return true;
    }

    bool finalize()
    {
        hid::finalize();

        return true;
    }

private:
    LoopTimer m_loopTimer;
    GameInput m_gameInput;
};


[[nodiscard]] String moduleName()
{
    return "hid";
}

[[nodiscard]] bool moduleEntry(
    SharedPtr<modules::Entry>& moduleEntry,
    modules::RuntimeModules& runtimeModules)
{
    return HIDEntry::createShared(
        moduleEntry,
        runtimeModules);
}


} // namespace hid
} // namespace waffle


