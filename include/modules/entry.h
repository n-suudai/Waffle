#pragma once


#include "modules/runtimeModules.h"


namespace waffle {
namespace modules {


class Entry : public ModulesEntity
{
public:
    Entry(RuntimeModules& modules);

    virtual ~Entry() = default;

    virtual bool entry() = 0;

    template<typename T>
    inline const T* As() const
    {
        const T* result = nullptr;

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
    bool moduleEntry(ModuleEntry entry, const EntryMethod& entryMethod);

    RuntimeModules& m_modules;
};


} // namespace modules
} // namespace waffle

