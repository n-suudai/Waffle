﻿
// System
#include "system/memory/memory.h"
#include "system/module_entry/module_entry.h"
#include "entity.h"
#include "runtime_heap.h"


// Modules
#include "modules/core/core.h"
#include "modules/application/application.h"
#include "modules/hid/hid.h"
#include "modules/render/render.h"


namespace waffle {


class Runtime : public RuntimeEntity
{
public:
    bool initialize()
    {
        m_modules = WFL_MAKE_UNIQUE(modules::RuntimeModules);

        Vector<SharedPtr<modules::Entry>> modules;

        SharedPtr<modules::Entry> entry;
        String moduleName;

        if (!core::moduleEntry(entry, *m_modules)) { return false; }
        
        modules.push_back(entry);

        if (!application::moduleEntry(entry, *m_modules)) { return false; }

        modules.push_back(entry);

        if (!hid::moduleEntry(entry, *m_modules)) { return false; }

        modules.push_back(entry);

        if (!render::moduleEntry(entry, *m_modules)) { return false; }

        modules.push_back(entry);

        if (!m_modules->initialize(wfl::move(modules))) { return false; }

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
        
        m_modules->execute(EntryPoint::InitializeScene);

        m_modules->execute(EntryPoint::Setup);

        m_modules->execute(EntryPoint::Start);

        while (m_modules->execute(EntryPoint::Update))
        {
            m_modules->execute(EntryPoint::UpdateRemoteHost);
            
            m_modules->execute(EntryPoint::UpdateScene);

            m_modules->execute(EntryPoint::WaitRenderring);

            m_modules->execute(EntryPoint::BeginRenderring);
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

