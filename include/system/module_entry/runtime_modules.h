#pragma once


#include "system/module_entry/entity.h"
#include "system/module_entry/entry_point.h"
#include "system/module_entry/entry_method_list.h"


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

    [[nodiscard]] bool entry(EntryPoint entry, const EntryMethod& entryMethod);

    bool execute(EntryPoint entry, bool reverse = false);

private:
    wfl::array<EntryMethodList, static_cast<wfl::size_t>(EntryPoint::Num)> m_entryPointArray;
    UnorderedMap<String, SharedPtr<Entry>> m_moduleMap;
};


} // namespace modules
} // namespace waffle

