#include "system/module_entry/entry_method_list.h"


namespace waffle {
namespace modules {


void EntryMethodList::operator +=(const EntryMethod& entryMethod)
{
    m_entryMethods.push_back(entryMethod);
}

bool EntryMethodList::operator()(bool reverse) const
{
    if (reverse)
    {
        return callReverse();
    }
    return call();
}

void EntryMethodList::clear()
{
    m_entryMethods.clear();
}

bool EntryMethodList::call() const
{
    Vector<EntryMethod>::const_iterator it = m_entryMethods.cbegin();
    Vector<EntryMethod>::const_iterator end = m_entryMethods.cend();
    while (it != end)
    {
        if (!(*it)())
        {
            return false;
        }
        ++it;
    }
    return true;
}

bool EntryMethodList::callReverse() const
{
    Vector<EntryMethod>::const_reverse_iterator it = m_entryMethods.crbegin();
    Vector<EntryMethod>::const_reverse_iterator end = m_entryMethods.crend();
    while (it != end)
    {
        if (!(*it)())
        {
            return false;
        }
        ++it;
    }
    return true;
}


} // namespace modules
} // namespace waffle

