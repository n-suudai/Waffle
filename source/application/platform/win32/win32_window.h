#pragma once


#include "application/window.h"


namespace waffle {
namespace application {


class Win32Window final : public IWindow
{
public:
    static memory::UniquePtr<IWindow> createUnique(const Rectangle<wfl::uint32_t>& clientRect);

    static memory::SharedPtr<IWindow> createShared(const Rectangle<wfl::uint32_t>& clientRect);

public:
    Win32Window(const Rectangle<wfl::uint32_t>& clientRect);

    ~Win32Window();

    virtual bool IsAlive() const override;

private:
    Rectangle<wfl::uint32_t> m_clientRect;
};


} // namespace application
} // namespace waffle

