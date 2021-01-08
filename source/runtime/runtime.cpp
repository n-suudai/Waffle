
// System
#include "system/memory/memory.h"
#include "system/module_entry/module_entry.h"
#include "entity.h"
#include "runtime_heap.h"


// Modules
#include "modules/core/core.h"
#include "modules/application/application.h"
#include "modules/hid/hid.h"


namespace waffle {


class Runtime : public RuntimeEntity
{
public:
    bool initialize()
    {
        m_modules = WFL_MAKE_UNIQUE(modules::RuntimeModules);

        UnorderedMap<String, SharedPtr<modules::Entry>> moduleMap;

        SharedPtr<modules::Entry> entry;
        String moduleName;

        if (!core::moduleEntry(entry, *m_modules)) { return false; }
        
        moduleName = core::moduleName();
        moduleMap[moduleName] = entry;

        if (!application::moduleEntry(entry, *m_modules)) { return false; }

        moduleName = application::moduleName();
        moduleMap[moduleName] = entry;

        if (!hid::moduleEntry(entry, *m_modules)) { return false; }

        moduleName = hid::moduleName();
        moduleMap[moduleName] = entry;

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

