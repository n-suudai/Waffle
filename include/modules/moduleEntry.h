#pragma once


namespace waffle {
namespace modules {


enum class ModuleEntry
{
    Initialize,
    //InitializeStorage,
    //InitializeResourceManager,
    //InitializeScene,
    //InitializeVM,

    Setup,

    Start,

    Update,

    Terminate,

    Finalize,

    Num,
};


} // namespace modules
} // namespace waffle

