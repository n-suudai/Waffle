
#include "memory.h"
#include "core.h"
#include "application.h"
#include "hid.h"
#include "render.h"

#include "runtime_heap.h"
#include "common/utility/loop_timer.h"
#include "game_input.h"

#include <functional>


namespace waffle {




enum class ModuleEntry
{
    Initialize,
    //InitializeStorage,
    //InitializeResourceManager,
    //InitializeScene,
    //InitializeVM,

    Setup,

    Start,

    Update,

    Terminate,

    Finalize,

    Num,
};


typedef wfl::function<bool()> EntryMethod;

class EntryPoint
{
public:
    EntryPoint() = default;
    ~EntryPoint() = default;

    void operator +=(const EntryMethod& entryMethod);

    bool operator()(bool reverse = false);

private:
    Vector<EntryMethod> m_entryMethodArray;
};


class Entry;
class RuntimeModules : public RuntimeEntity
{
public:
    bool initialize();

    bool finalize();

    bool entryAll();

    bool getModule(const String& moduleName, WeakPtr<Entry>& outModule);

    bool entry(ModuleEntry entry, const EntryMethod& entryMethod);

    bool execute(ModuleEntry entry, bool reverse = false);

private:
    wfl::array<EntryPoint, static_cast<wfl::size_t>(ModuleEntry::Num)> m_entryPointArray;
    UnorderedMap<String, SharedPtr<Entry>> m_moduleMap;
};


class Entry : public RuntimeEntity
{
public:
    Entry(RuntimeModules& modules)
        : m_modules(modules)
    {}

    virtual ~Entry() = default;

    virtual bool entry() = 0;

    template<typename T>
    T* As()
    {
        return dynamic_cast<T*>(this);
    }

protected:
    bool moduleEntry(ModuleEntry entry, const EntryMethod& entryMethod);

    RuntimeModules& m_modules;
};

bool Entry::moduleEntry(ModuleEntry entry, const EntryMethod& entryMethod)
{
    return m_modules.entry(entry, entryMethod);
}


class CoreEntry : public Entry
{
public:
    CoreEntry(RuntimeModules& modules)
        : Entry(modules)
    {}

    bool entry() override;

private:
    bool initialize();

    bool finalize();
};


class ApplicationEntry : public Entry
{
public:
    ApplicationEntry(RuntimeModules& modules)
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


class HIDEntry : public Entry
{
public:
    HIDEntry(RuntimeModules& modules)
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


class Runtime
{
public:
    bool initialize()
    {
        if (!memory::initialize()) { return false; }

        m_modules = WFL_GLOBAL_HEAP_MAKE_UNIQUE(RuntimeModules);

        if (!m_modules->initialize()) { return false; }
        
        if (!m_modules->entryAll()) { return false; }

        return true;
    }

    bool finalize()
    {
        if (!m_modules->finalize()) { return false; }

        m_modules.reset();

        runtime::RuntimeHeap::printDebug_Report_MemoryAll(); // メモリの状態を表示

        memory::finalize();

        return true;
    }

    void run()
    {
        m_modules->execute(ModuleEntry::Initialize);

        m_modules->execute(ModuleEntry::Setup);

        m_modules->execute(ModuleEntry::Start);

        while (m_modules->execute(ModuleEntry::Update))
        {

        }

        m_modules->execute(ModuleEntry::Terminate, true);

        m_modules->execute(ModuleEntry::Finalize, true);
    }

private:
    UniquePtr<RuntimeModules> m_modules;
};


// EntryPoint
void EntryPoint::operator +=(const EntryMethod& entryMethod)
{
    m_entryMethodArray.push_back(entryMethod);
}

bool EntryPoint::operator()(bool reverse)
{
    if (!reverse)
    {
        Vector<EntryMethod>::iterator it = m_entryMethodArray.begin();
        Vector<EntryMethod>::const_iterator end = m_entryMethodArray.cend();
        while (it != end)
        {
            if (!(*it)())
            {
                return false;
            }
            ++it;
        }
    }
    else
    {
        Vector<EntryMethod>::reverse_iterator it = m_entryMethodArray.rbegin();
        Vector<EntryMethod>::const_reverse_iterator end = m_entryMethodArray.crend();
        while (it != end)
        {
            if (!(*it)())
            {
                return false;
            }
            ++it;
        }
    }

    return true;
}


// RuntimeModules
bool RuntimeModules::initialize()
{
    m_moduleMap.insert({ "Core", WFL_MAKE_SHARED(CoreEntry, *this) });
    m_moduleMap.insert({ "Application", WFL_MAKE_SHARED(ApplicationEntry, *this) });
    m_moduleMap.insert({ "HID", WFL_MAKE_SHARED(HIDEntry, *this) });
    return true;
}

bool RuntimeModules::finalize()
{
    m_moduleMap.clear();
    return true;
}

bool RuntimeModules::entryAll()
{
    for (Map<String, SharedPtr<Entry>>::value_type& pair : m_moduleMap)
    {
        if (!pair.second->entry())
        {
            return false;
        }
    }

    return true;
}

bool RuntimeModules::getModule(const String& moduleName, WeakPtr<Entry>& outModule)
{
    UnorderedMap<String, SharedPtr<Entry>>::iterator it = m_moduleMap.find(moduleName);

    if (it != m_moduleMap.end())
    {
        outModule = it->second;
        return true;
    }

    return false;
}

bool RuntimeModules::entry(ModuleEntry entry, const EntryMethod& entryMethod)
{
    m_entryPointArray[static_cast<wfl::size_t>(entry)] += entryMethod;
    return true;
}

bool RuntimeModules::execute(ModuleEntry entry, bool reverse)
{
    return m_entryPointArray[static_cast<wfl::size_t>(entry)](reverse);
}


// Core
bool CoreEntry::entry()
{
    if (!moduleEntry(ModuleEntry::Initialize, wfl::bind(&CoreEntry::initialize, this)))
    {
        return false;
    }

    if (!moduleEntry(ModuleEntry::Finalize, wfl::bind(&CoreEntry::finalize, this)))
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
    if (!moduleEntry(ModuleEntry::Initialize, wfl::bind(&ApplicationEntry::initialize, this)))
    {
        return false;
    }

    if (!moduleEntry(ModuleEntry::Setup, wfl::bind(&ApplicationEntry::setup, this)))
    {
        return false;
    }

    if (!moduleEntry(ModuleEntry::Update, wfl::bind(&ApplicationEntry::update, this)))
    {
        return false;
    }

    if (!moduleEntry(ModuleEntry::Finalize, wfl::bind(&ApplicationEntry::finalize, this)))
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
    if (!moduleEntry(ModuleEntry::Initialize, wfl::bind(&HIDEntry::initialize, this)))
    {
        return false;
    }

    if (!moduleEntry(ModuleEntry::Setup, wfl::bind(&HIDEntry::setup, this)))
    {
        return false;
    }

    if (!moduleEntry(ModuleEntry::Update, wfl::bind(&HIDEntry::update, this)))
    {
        return false;
    }

    if (!moduleEntry(ModuleEntry::Terminate, wfl::bind(&HIDEntry::terminate, this)))
    {
        return false;
    }

    if (!moduleEntry(ModuleEntry::Finalize, wfl::bind(&HIDEntry::finalize, this)))
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

    ApplicationEntry* pApplication = sharedApplication->As<ApplicationEntry>();

    if (!pApplication) { return false; }

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


wfl::int32_t runtimeMain()
{
    Runtime runtime;

    if (!runtime.initialize()) { return 0; }

    runtime.run();

    runtime.finalize();

    return 0;
}


} // namespace runtime
} // namespace waffle

