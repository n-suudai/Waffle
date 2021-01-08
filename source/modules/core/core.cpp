#include "modules/core/core.h"


namespace waffle {
namespace core {


class CoreEntry : public modules::Entry
{
public:
    [[nodiscard]] static bool createShared(
        SharedPtr<modules::Entry>& moduleEntry,
        modules::RuntimeModules& runtimeModules)
    {
        SharedPtr<CoreEntry> ptr = WFL_MAKE_SHARED(CoreEntry, runtimeModules);
        if (ptr)
        {
            moduleEntry = wfl::move(ptr);
            return true;
        }
        return false;
    }

    CoreEntry(modules::RuntimeModules& modules)
        : Entry(modules)
    {}

    bool entry() override
    {
        using namespace modules;

        if (!moduleEntry(EntryPoint::Initialize, wfl::bind(&CoreEntry::initialize, this)))
        {
            return false;
        }

        if (!moduleEntry(EntryPoint::Finalize, wfl::bind(&CoreEntry::finalize, this)))
        {
            return false;
        }

        return true;
    }

private:


    bool initialize()
    {
        return core::initialize();
    }

    bool finalize()
    {
        core::finalize();
        return true;
    }
};


bool initialize()
{
    logging::put("[core] initialized.");

    return true;
}

String moduleName()
{
    return "core";
}

bool moduleEntry(
    SharedPtr<modules::Entry>& moduleEntry,
    modules::RuntimeModules& runtimeModules)
{
    return CoreEntry::createShared(
        moduleEntry,
        runtimeModules);
}

void finalize()
{
    logging::put("[core] finalized.");
}


} // namespace core
} // namespace waffle
