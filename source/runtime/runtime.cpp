
#include "system/memory/memory.h"
#include "system/module_entry/module_entry.h"

#include "runtime_heap.h"
#include "system/common/utility/loop_timer.h"
#include "game_input.h"

#include <functional>


namespace waffle {


class CoreEntry : public modules::Entry
{
public:
    CoreEntry(modules::RuntimeModules& modules)
        : Entry(modules)
    {}

    bool entry() override;

private:
    bool initialize();

    bool finalize();
};


class ApplicationEntry : public modules::Entry
{
public:
    ApplicationEntry(modules::RuntimeModules& modules)
        : Entry(modules)
    {}

    bool entry() override;

    const UniquePtr<application::IWindow>& window() const;

private:
    bool initialize();

    bool setup();

    bool update();

    bool finalize();

private:
    UniquePtr<application::IWindow> m_window;
};


class HIDEntry : public modules::Entry
{
public:
    HIDEntry(modules::RuntimeModules& modules)
        : Entry(modules)
    {}

    bool entry() override;

private:
    bool initialize();

    bool setup();

    bool update();

    bool terminate();

    bool finalize();

private:
    LoopTimer m_loopTimer;
    GameInput m_gameInput;
};


class Runtime : public RuntimeEntity
{
public:
    bool initialize()
    {
        m_modules = WFL_MAKE_UNIQUE(modules::RuntimeModules);

        UnorderedMap<String, SharedPtr<modules::Entry>> moduleMap;
        moduleMap["Core"] = WFL_MAKE_SHARED(CoreEntry, (*m_modules));
        moduleMap["Application"] = WFL_MAKE_SHARED(ApplicationEntry, (*m_modules));
        moduleMap["HID"] = WFL_MAKE_SHARED(HIDEntry, (*m_modules));

        if (!m_modules->initialize(wfl::move(moduleMap))) { return false; }
        
        if (!m_modules->entryAll()) { return false; }

        return true;
    }

    bool finalize()
    {
        if (!m_modules->finalize()) { return false; }

        m_modules.reset();

        return true;
    }

    void run()
    {
        using namespace modules;

        m_modules->execute(EntryPoint::Initialize);

        m_modules->execute(EntryPoint::Setup);

        m_modules->execute(EntryPoint::Start);

        while (m_modules->execute(EntryPoint::Update))
        {

        }

        m_modules->execute(EntryPoint::Terminate, true);

        m_modules->execute(EntryPoint::Finalize, true);
    }

private:
    UniquePtr<modules::RuntimeModules> m_modules;
};


// Core
bool CoreEntry::entry()
{
    using namespace modules;

    if (!moduleEntry(EntryPoint::Initialize, wfl::bind(&CoreEntry::initialize, this)))
    {
        return false;
    }

    if (!moduleEntry(EntryPoint::Finalize, wfl::bind(&CoreEntry::finalize, this)))
    {
        return false;
    }

    return true;
}

bool CoreEntry::initialize()
{
    return core::initialize();
}

bool CoreEntry::finalize()
{
    core::finalize();
    return true;
}


// Application
bool ApplicationEntry::entry()
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

const UniquePtr<application::IWindow>& ApplicationEntry::window() const
{
    return m_window;
}

bool ApplicationEntry::initialize()
{
    return application::initialize();
}

bool ApplicationEntry::setup()
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

bool ApplicationEntry::update()
{
    if (!m_window) { return false; }

    if (!m_window->isAlive()) { return false; }

    return m_window->messagePump();
}

bool ApplicationEntry::finalize()
{
    m_window.reset();
    application::finalize();
    return true;
}


// HID
bool HIDEntry::entry()
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

bool HIDEntry::initialize()
{
    return hid::initialize();
}

bool HIDEntry::setup()
{
    WeakPtr<Entry> weakApplication;
    if (!m_modules.getModule("Application", weakApplication))
    {
        return false;
    }

    SharedPtr<Entry> sharedApplication = weakApplication.lock();

    if (!sharedApplication) { return false; }

    const ApplicationEntry* pApplication = nullptr;

    if (!sharedApplication->As<ApplicationEntry>(pApplication)) { return false; }

    if (!m_gameInput.initialize(pApplication->window())) { return false; }

    m_loopTimer = LoopTimer();

    return true;
}

bool HIDEntry::update()
{
    hid::Duration elapsed = m_loopTimer.update<hid::Duration>();

    m_gameInput.update(elapsed);

    if (m_gameInput.isAnyPressed()) { return false; }

    return true;
}

bool HIDEntry::terminate()
{
    if (!m_gameInput.finalize()) { return false; }

    return true;
}

bool HIDEntry::finalize()
{
    hid::finalize();

    return true;
}


} // namespace waffle


namespace waffle {
namespace runtime {


class MemoryInitializer final
{
public:
    MemoryInitializer()
    {
        if (!memory::initialize())
        {
            abort();
        }
    }

    ~MemoryInitializer()
    {
        runtime::RuntimeHeap::printDebug_Report_MemoryAll(); // メモリの状態を表示
        memory::finalize();
    }
};


bool runtimeBody()
{
    Runtime runtime;

    if (!runtime.initialize()) { return false; }

    runtime.run();

    return runtime.finalize();
}


wfl::int32_t runtimeMain()
{
    MemoryInitializer memoryInitializer;

    runtimeBody();

    return 0;
}


} // namespace runtime
} // namespace waffle

