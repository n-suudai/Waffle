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
   
    template<typename T>
    [[nodiscard]] inline bool As(const T*& result) const
    {
        try
        {
            result = dynamic_cast<const T*>(this);
        }
        catch (wfl::bad_cast&)
        {
            result = nullptr;
        }

        return result;
    }

protected:
    [[nodiscard]] bool moduleEntry(EntryPoint entry, const EntryMethod& entryMethod);

    RuntimeModules& m_modules;
};


} // namespace modules
} // namespace waffle

