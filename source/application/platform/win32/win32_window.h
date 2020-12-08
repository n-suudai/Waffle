#pragma once


#include "application/window.h"


namespace waffle {
namespace application {


class Win32Window final : public IWindow
{
public:
    static memory::UniquePtr<IWindow> createUnique();

    static memory::SharedPtr<IWindow> createShared();

public:
    Win32Window();

    ~Win32Window();

    virtual bool IsAlive() const override;
};


} // namespace application
} // namespace waffle

