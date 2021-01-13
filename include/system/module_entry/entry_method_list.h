#pragma once


#include "system/module_entry/entity.h"
#include <functional>


namespace waffle {
namespace modules {


typedef wfl::function<bool()> EntryMethod;

class EntryMethodList final : public ModulesEntity
{
public:
    EntryMethodList() = default;
    ~EntryMethodList() = default;

    void operator +=(const EntryMethod& entryMethod);

    [[nodiscard]] bool operator()(bool reverse = false) const;

    void clear();

private:
    [[nodiscard]] bool call() const;

    [[nodiscard]] bool callReverse() const;

private:
    Vector<EntryMethod> m_entryMethods;
};


} // namespace modules
} // namespace waffle

