#pragma once

#include "hid/mouse.h"
#include "hid/input_state.h"
#include "direct_input.h"


namespace waffle {
namespace hid {


class DInputMouse final : public IMouse
{
public:
    static bool createUnique(
        const ComPtr<IDirectInputDevice8A>& device,
        HWND hWindow,
        UniquePtr<IMouse>& outMouse);

    static bool createShared(
        const ComPtr<IDirectInputDevice8A>& device,
        HWND hWindow,
        SharedPtr<IMouse>& outMouse);

public:
    DInputMouse();

    ~DInputMouse();

    void update(const Duration& deltaTime) override;

    bool isAnyPressed() const override;

    bool isPressed(MouseButton button) const override;

    bool isReleased(MouseButton button) const override;

    bool isFirstPressed(MouseButton button) const override;

    bool isFirstReleased(MouseButton button) const override;

    Duration durationPressed(MouseButton button) const override;

    wfl::int32_t axisValue(MouseAxis axis) const override;

private:
    bool initialize(
        const ComPtr<IDirectInputDevice8A>& device,
        HWND hWindow);

private:
    typedef wfl::array<InputState, MouseButton::MAX_NUM> InputStateTable;

    ComPtr<IDirectInputDevice8A> m_device;
    HWND m_hWindow;
    wfl::array<InputStateTable, 2> m_stateTables;
    wfl::array<Duration, MouseButton::MAX_NUM> m_pressedDurations;
    wfl::array<wfl::int32_t, MouseAxis::MAX_NUM> m_axisValues;
};


} // namespace hid
} // namespace waffle

