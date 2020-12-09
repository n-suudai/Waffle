#pragma once


#include "application/entity.h"
#include "common/utility/rectangle.h"


namespace waffle {
namespace application {


class IWindow : public ApplicationEntity
{
public:
    virtual bool isAlive() const = 0;

    virtual bool messagePump() = 0;
};


memory::UniquePtr<IWindow> createWindowUnique(const Rectangle<wfl::int32_t>& clientRect);

memory::SharedPtr<IWindow> createWindowShared(const Rectangle<wfl::int32_t>& clientRect);


} // namespace application
} // namespace waffle

