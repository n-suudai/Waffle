#include "modules/entry.h"


namespace waffle {
namespace modules {


bool RuntimeModules::initialize(UnorderedMap<String, SharedPtr<Entry>>&& moduleMap)
{
    m_moduleMap = wfl::move(moduleMap);
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


} // namespace modules
} // namespace waffle

