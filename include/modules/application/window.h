#pragma once


#include "modules/application/entity.h"
#include "system/common/utility/rectangle.h"

#include <functional>


namespace waffle {
namespace application {


class IWindow : public ApplicationEntity
{
public:
    [[nodiscard]] virtual bool isAlive() const = 0;

    virtual bool messagePump() = 0;

    virtual bool setTitle(const StringView& title) = 0;

    virtual bool setClientRect(const Rectangle<wfl::int32_t>& clientRect) = 0;

    [[nodiscard]] virtual void* windowHandle() const = 0;

    [[nodiscard]] virtual void* applicationHandle() const = 0;
};


[[nodiscard]] bool createWindowUnique(
    const Rectangle<wfl::int32_t>& clientRect,
    UniquePtr<IWindow>& outWindow);

[[nodiscard]] bool createWindowShared(
    const Rectangle<wfl::int32_t>& clientRect,
    SharedPtr<IWindow>& outWindow);


} // namespace application
} // namespace waffle

