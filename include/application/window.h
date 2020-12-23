﻿#pragma once


#include "application/entity.h"
#include "common/utility/rectangle.h"
#include "memory/stl.h"


namespace waffle {
namespace application {


class IWindow : public ApplicationEntity
{
public:
    virtual bool isAlive() const = 0;

    virtual bool messagePump() = 0;

    virtual bool setTitle(const StringView& title) = 0;

    virtual bool setClientRect(const Rectangle<wfl::int32_t>& clientRect) = 0;

    virtual void* windowHandle() const = 0;

    virtual void* applicationHandle() const = 0;
};


UniquePtr<IWindow> createWindowUnique(const Rectangle<wfl::int32_t>& clientRect);

SharedPtr<IWindow> createWindowShared(const Rectangle<wfl::int32_t>& clientRect);


} // namespace application
} // namespace waffle

