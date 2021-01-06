#include "modules/entry.h"


namespace waffle {
namespace modules {


Entry::Entry(RuntimeModules& modules)
    : m_modules(modules)
{}

bool Entry::moduleEntry(ModuleEntry entry, const EntryMethod& entryMethod)
{
    return m_modules.entry(entry, entryMethod);
}


} // namespace modules
} // namespace waffle

