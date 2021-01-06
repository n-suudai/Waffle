#pragma once

#include "modules/hid/keyboard/keyboard.h"
#include "modules/hid/input_state.h"
#include "direct_input.h"


namespace waffle {
namespace hid {


class DInputKeyboard final : public IKeyboard
{
public:
    static void buildKeyMapTable();

    [[nodiscard]] static bool createUnique(
        const ComPtr<IDirectInputDevice8A>& device,
        HWND hWindow,
        UniquePtr<IKeyboard>& outKeyboard);

    [[nodiscard]] static bool createShared(
        const ComPtr<IDirectInputDevice8A>& device,
        HWND hWindow,
        SharedPtr<IKeyboard>& outKeyboard);

public:
    DInputKeyboard();

    ~DInputKeyboard();

    void update(const Duration& deltaTime) override;

    [[nodiscard]] bool isAnyPressed() const override;

    [[nodiscard]] bool isPressed(KeyCode key) const override;

    [[nodiscard]] bool isReleased(KeyCode key) const override;

    [[nodiscard]] bool isFirstPressed(KeyCode key) const override;

    [[nodiscard]] bool isFirstReleased(KeyCode key) const override;

    [[nodiscard]] Duration durationPressed(KeyCode key) const override;

private:
    [[nodiscard]] bool initialize(
        const ComPtr<IDirectInputDevice8A>& device,
        HWND hWindow);

private:
    typedef wfl::array<InputState, KeyCode::MAX_NUM> InputStateTable;

    ComPtr<IDirectInputDevice8A> m_device;
    HWND m_hWindow;

    wfl::array<InputStateTable, 2> m_stateTables;

    wfl::array<Duration, KeyCode::MAX_NUM> m_pressedDurations;

    static wfl::array<wfl::uint8_t, KeyCode::MAX_NUM> s_keyMapTable;
};


} // namespace hid
} // namespace waffle

