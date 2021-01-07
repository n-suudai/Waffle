#pragma once


namespace waffle {
namespace modules {


enum class EntryPoint
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

