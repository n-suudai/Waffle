#pragma once


#include "system/module_entry/runtime_modules.h"


namespace waffle {
namespace modules {


class Entry : public ModulesEntity
{
public:
    Entry(RuntimeModules& modules);

    virtual ~Entry() = default;

    [[nodiscard]] virtual bool entry() = 0;

    [[nodiscard]] virtual String moduleName() const = 0;

    [[nodiscard]] virtual const void* getProperty(const String& name) const = 0;

protected:
    [[nodiscard]] bool moduleEntry(EntryPoint entry, const EntryMethod& entryMethod);

    RuntimeModules& m_modules;
};


} // namespace modules
} // namespace waffle

