#include "system/module_entry/entry.h"


namespace waffle {
namespace modules {


bool RuntimeModules::initialize(Vector<SharedPtr<Entry>>&& modules)
{
    clearEntryPoints();

    m_modules = wfl::move(modules);

    return true;
}

bool RuntimeModules::finalize()
{
    clearEntryPoints();

    m_modules.clear();

    return true;
}

bool RuntimeModules::entryAll()
{
    for (SharedPtr<Entry>& entryModule : m_modules)
    {
        if (!entryModule->entry())
        {
            return false;
        }
    }

    return true;
}

bool RuntimeModules::getModule(const String& moduleName, WeakPtr<Entry>& outModule)
{
    Vector<SharedPtr<Entry>>::iterator it = wfl::find_if(
        m_modules.begin(),
        m_modules.end(),
        [moduleName](const SharedPtr<Entry>& entryModule) -> bool
        {
            return entryModule->moduleName() == moduleName;
        });

    if (it != m_modules.end())
    {
        outModule = (*it);
        return true;
    }

    return false;
}

bool RuntimeModules::entry(EntryPoint entry, const EntryMethod& entryMethod)
{
    m_entryPoints[static_cast<wfl::size_t>(entry)] += entryMethod;
    return true;
}

bool RuntimeModules::execute(EntryPoint entry, bool reverse)
{
    return m_entryPoints[static_cast<wfl::size_t>(entry)](reverse);
}

void RuntimeModules::clearEntryPoints()
{
    wfl::for_each(
        m_entryPoints.begin(),
        m_entryPoints.end(),
        [](EntryMethodList& methodList)
        {
            methodList.clear();
        });
}


} // namespace modules
} // namespace waffle

