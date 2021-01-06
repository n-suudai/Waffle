#pragma once


#include "modules/entity.h"
#include <functional>


namespace waffle {
namespace modules {


typedef wfl::function<bool()> EntryMethod;

class EntryPoint final : public ModulesEntity
{
public:
    EntryPoint() = default;
    ~EntryPoint() = default;

    void operator +=(const EntryMethod& entryMethod);

    bool operator()(bool reverse = false);

private:
    Vector<EntryMethod> m_entryMethodArray;
};


} // namespace modules
} // namespace waffle

