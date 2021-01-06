#pragma once

#include "modules/hid/mouse/mouse.h"
#include "modules/hid/input_state.h"
#include "direct_input.h"


namespace waffle {
namespace hid {


class DInputMouse final : public IMouse
{
public:
    [[nodiscard]] static bool createUnique(
        const ComPtr<IDirectInputDevice8A>& device,
        HWND hWindow,
        UniquePtr<IMouse>& outMouse);

    [[nodiscard]] static bool createShared(
        const ComPtr<IDirectInputDevice8A>& device,
        HWND hWindow,
        SharedPtr<IMouse>& outMouse);

public:
    DInputMouse();

    ~DInputMouse();

    void update(const Duration& deltaTime) override;

    [[nodiscard]] bool isAnyPressed() const override;

    [[nodiscard]] bool isPressed(MouseButton button) const override;

    [[nodiscard]] bool isReleased(MouseButton button) const override;

    [[nodiscard]] bool isFirstPressed(MouseButton button) const override;

    [[nodiscard]] bool isFirstReleased(MouseButton button) const override;

    [[nodiscard]] Duration durationPressed(MouseButton button) const override;

    [[nodiscard]] wfl::int32_t axisValue(MouseAxis axis) const override;

private:
    [[nodiscard]] bool initialize(
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

