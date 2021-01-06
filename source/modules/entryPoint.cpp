#include "modules/entryPoint.h"


namespace waffle {
namespace modules {


void EntryPoint::operator +=(const EntryMethod& entryMethod)
{
    m_entryMethodArray.push_back(entryMethod);
}

bool EntryPoint::operator()(bool reverse)
{
    if (!reverse)
    {
        Vector<EntryMethod>::iterator it = m_entryMethodArray.begin();
        Vector<EntryMethod>::const_iterator end = m_entryMethodArray.cend();
        while (it != end)
        {
            if (!(*it)())
            {
                return false;
            }
            ++it;
        }
    }
    else
    {
        Vector<EntryMethod>::reverse_iterator it = m_entryMethodArray.rbegin();
        Vector<EntryMethod>::const_reverse_iterator end = m_entryMethodArray.crend();
        while (it != end)
        {
            if (!(*it)())
            {
                return false;
            }
            ++it;
        }
    }

    return true;
}


} // namespace modules
} // namespace waffle

