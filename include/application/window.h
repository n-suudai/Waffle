#pragma once


#include "application/entity.h"


namespace waffle {
namespace application {


class IWindow : public ApplicationEntity
{
public:
    virtual bool IsAlive() const = 0;
};


memory::UniquePtr<IWindow> createWindowUnique();

memory::SharedPtr<IWindow> createWindowShared();


} // namespace application
} // namespace waffle

