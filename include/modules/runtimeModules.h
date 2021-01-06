#pragma once


#include "modules/entity.h"
#include "modules/moduleEntry.h"
#include "modules/entryPoint.h"


namespace waffle {
namespace modules {


class Entry;
class RuntimeModules : public ModulesEntity
{
public:
    [[nodiscard]] bool initialize(UnorderedMap<String, SharedPtr<Entry>>&& moduleMap);

    [[nodiscard]] bool finalize();

    [[nodiscard]] bool entryAll();

    [[nodiscard]] bool getModule(const String& moduleName, WeakPtr<Entry>& outModule);

    [[nodiscard]] bool entry(ModuleEntry entry, const EntryMethod& entryMethod);

    bool execute(ModuleEntry entry, bool reverse = false);

private:
    wfl::array<EntryPoint, static_cast<wfl::size_t>(ModuleEntry::Num)> m_entryPointArray;
    UnorderedMap<String, SharedPtr<Entry>> m_moduleMap;
};


} // namespace modules
} // namespace waffle

