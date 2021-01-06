#include "modules/entryPoint.h"


namespace waffle {
namespace modules {


void EntryPoint::operator +=(const EntryMethod& entryMethod)
{
    m_entryMethodArray.push_back(entryMethod);
}

bool EntryPoint::operator()(bool reverse) const
{
    if (reverse)
    {
        return callReverse();
    }
    return call();
}

bool EntryPoint::call() const
{
    Vector<EntryMethod>::const_iterator it = m_entryMethodArray.cbegin();
    Vector<EntryMethod>::const_iterator end = m_entryMethodArray.cend();
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

bool EntryPoint::callReverse() const
{
    Vector<EntryMethod>::const_reverse_iterator it = m_entryMethodArray.crbegin();
    Vector<EntryMethod>::const_reverse_iterator end = m_entryMethodArray.crend();
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

