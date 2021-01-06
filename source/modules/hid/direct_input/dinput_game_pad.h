#pragma once

#include "modules/hid/game_pad/game_pad.h"
#include "modules/hid/input_state.h"
#include "direct_input.h"


namespace waffle {
namespace hid {


class DInputGamePad final : public IGamePad
{
public:
    [[nodiscard]] static bool createUnique(
        const ComPtr<IDirectInputDevice8A>& device,
        HWND hWindow,
        UniquePtr<IGamePad>& outGamePad);

    [[nodiscard]] static bool createShared(
        const ComPtr<IDirectInputDevice8A>& device,
        HWND hWindow,
        SharedPtr<IGamePad>& outGamePad);

public:
    DInputGamePad();

    ~DInputGamePad();

    void update(const Duration& deltaTime) override;

    [[nodiscard]]bool isAnyPressed() const override;
    [[nodiscard]]bool isAnyButtonPressed() const override;
    [[nodiscard]]bool isAnyPOVPressed() const override;

    [[nodiscard]] bool isPressed(GamePadButton button) const override;
    [[nodiscard]] bool isPressed(POV pov) const override;

    [[nodiscard]] bool isReleased(GamePadButton button) const override;
    [[nodiscard]] bool isReleased(POV pov) const override;

    [[nodiscard]] bool isFirstPressed(GamePadButton button) const override;
    [[nodiscard]] bool isFirstPressed(POV pov) const override;

    [[nodiscard]] bool isFirstReleased(GamePadButton button) const override;
    [[nodiscard]] bool isFirstReleased(POV pov) const override;

    [[nodiscard]] Duration durationPressed(GamePadButton button) const override;
    [[nodiscard]] Duration durationPressed(POV pov) const override;

    [[nodiscard]] wfl::int32_t analogInputValue(AnalogInput analog) const override;

private:
    [[nodiscard]] bool initialize(
        const ComPtr<IDirectInputDevice8A>& device,
        HWND hWindow);

    void updateAllState(const Duration& deltaTime, const DIJOYSTATE2& joystate2);

    void updateAnalogValues(const DIJOYSTATE2& joystate2);
    void updateButtons(const Duration& deltaTime, const DIJOYSTATE2& joystate2);
    void updatePOVs(const Duration& deltaTime, const DIJOYSTATE2& joystate2);

private:
    typedef wfl::array<InputState, GamePadButton::MAX_NUM> InputStateTableGamePadButton;
    typedef wfl::array<InputState, POV::MAX_NUM> InputStateTablePOV;
    typedef wfl::array<wfl::int32_t, AnalogInput::MAX_NUM> AnalogValueTable;

    ComPtr<IDirectInputDevice8A> m_device;
    HWND m_hWindow;

    wfl::array<InputStateTableGamePadButton, 2> m_buttonStateTables;
    wfl::array<Duration, GamePadButton::MAX_NUM> m_buttonPressedDurations;

    wfl::array<InputStateTablePOV, 2> m_povStateTables;
    wfl::array<Duration, POV::MAX_NUM> m_povPressedDurations;

    AnalogValueTable m_analogValues;
};


} // namespace hid
} // namespace waffle

