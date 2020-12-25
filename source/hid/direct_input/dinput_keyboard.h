#pragma once

#include "hid/keyboard/keyboard.h"
#include "hid/input_state.h"
#include "direct_input.h"


namespace waffle {
namespace hid {


class DInputKeyboard final : public IKeyboard
{
public:
    static void buildKeyMapTable();

    static bool createUnique(
        const ComPtr<IDirectInputDevice8A>& device,
        HWND hWindow,
        UniquePtr<IKeyboard>& outKeyboard);

    static bool createShared(
        const ComPtr<IDirectInputDevice8A>& device,
        HWND hWindow,
        SharedPtr<IKeyboard>& outKeyboard);

public:
    DInputKeyboard();

    ~DInputKeyboard();

    void update(const Duration& deltaTime) override;

    bool isAnyPressed() const override;

    bool isPressed(KeyCode key) const override;

    bool isReleased(KeyCode key) const override;

    bool isFirstPressed(KeyCode key) const override;

    bool isFirstReleased(KeyCode key) const override;

    Duration durationPressed(KeyCode key) const override;

private:
    bool initialize(
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

