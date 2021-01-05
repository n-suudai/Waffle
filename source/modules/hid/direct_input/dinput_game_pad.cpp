
#include "dinput_game_pad.h"
#include "dinput_utility.h"


namespace waffle {
namespace hid {


bool DInputGamePad::createUnique(
    const ComPtr<IDirectInputDevice8A>& device,
    HWND hWindow,
    UniquePtr<IGamePad>& outGamePad)
{
    UniquePtr<DInputGamePad> ptr = WFL_MAKE_UNIQUE(DInputGamePad);

    if (ptr->initialize(device, hWindow))
    {
        outGamePad = wfl::move(ptr);
        return true;
    }

    return false;
}

bool DInputGamePad::createShared(
    const ComPtr<IDirectInputDevice8A>& device,
    HWND hWindow,
    SharedPtr<IGamePad>& outGamePad)
{
    SharedPtr<DInputGamePad> ptr = WFL_MAKE_SHARED(DInputGamePad);

    if (ptr->initialize(device, hWindow))
    {
        outGamePad = wfl::move(ptr);
        return true;
    }

    return false;
}

DInputGamePad::DInputGamePad()
    : m_device()
    , m_hWindow(NULL)
    , m_buttonStateTables({ InputStateType::Released })
    , m_buttonPressedDurations({ DURATION_ZERO })
    , m_povStateTables({ InputStateType::Released })
    , m_povPressedDurations({ DURATION_ZERO })
    , m_analogValues({ 0 })
{}

DInputGamePad::~DInputGamePad()
{}

void DInputGamePad::update(const Duration& deltaTime)
{
    if (!m_device) { return; }

    if (!DInputUtility::isWindowAvailable(m_hWindow)) { return; }

    if (!DInputUtility::DeviceAcquire(m_device)) { return; }

    if (!DInputUtility::DevicePoll(m_device)) { return; }

    DIJOYSTATE2 joystate2 = { 0 };
    if (!DInputUtility::DeviceGetState(m_device, sizeof(joystate2), &joystate2)) { return; }

    updateAllState(deltaTime, joystate2);
}

bool DInputGamePad::isAnyPressed() const
{
    if (isAnyButtonPressed()) { return true; }
    if (isAnyPOVPressed()) { return true; }
    return false;
}

bool DInputGamePad::isAnyButtonPressed() const
{
    bool result = wfl::any_of(
        m_buttonStateTables[0].begin(),
        m_buttonStateTables[0].end(),
        [](const InputState& state) -> bool
        {
            return state.is(InputStateType::Pressed);
        });

    return result;
}

bool DInputGamePad::isAnyPOVPressed() const
{
    bool result = wfl::any_of(
        m_povStateTables[0].begin(),
        m_povStateTables[0].end(),
        [](const InputState& state) -> bool
        {
            return state.is(InputStateType::Pressed);
        });

    return result;
}

bool DInputGamePad::isPressed(GamePadButton button) const
{
    if (button.isValid()) { return false; }
    return m_buttonStateTables[0][button.toIndex()].is(InputStateType::Pressed);
}

bool DInputGamePad::isPressed(POV pov) const
{
    if (pov.isValid()) { return false; }
    return m_povStateTables[0][pov.toIndex()].is(InputStateType::Pressed);
}

bool DInputGamePad::isReleased(GamePadButton button) const
{
    if (!button.isValid()) { return false; }
    return m_buttonStateTables[0][button.toIndex()].is(InputStateType::Released);
}

bool DInputGamePad::isReleased(POV pov) const
{
    if (!pov.isValid()) { return false; }
    return m_povStateTables[0][pov.toIndex()].is(InputStateType::Released);
}

bool DInputGamePad::isFirstPressed(GamePadButton button) const
{
    if (!button.isValid()) { return false; }
    return m_buttonStateTables[0][button.toIndex()].is(InputStateType::Pressed)
        && m_buttonStateTables[1][button.toIndex()].is(InputStateType::Released);
}

bool DInputGamePad::isFirstPressed(POV pov) const
{
    if (!pov.isValid()) { return false; }
    return m_povStateTables[0][pov.toIndex()].is(InputStateType::Pressed)
        && m_povStateTables[1][pov.toIndex()].is(InputStateType::Released);
}

bool DInputGamePad::isFirstReleased(GamePadButton button) const
{
    if (!button.isValid()) { return false; }
    return m_buttonStateTables[0][button.toIndex()].is(InputStateType::Released)
        && m_buttonStateTables[1][button.toIndex()].is(InputStateType::Pressed);
}

bool DInputGamePad::isFirstReleased(POV pov) const
{
    if (!pov.isValid()) { return false; }
    return m_povStateTables[0][pov.toIndex()].is(InputStateType::Released)
        && m_povStateTables[1][pov.toIndex()].is(InputStateType::Pressed);
}

Duration DInputGamePad::durationPressed(GamePadButton button) const
{
    if (!button.isValid()) { return DURATION_ZERO; }
    return m_buttonPressedDurations[button.toIndex()];
}

Duration DInputGamePad::durationPressed(POV pov) const
{
    if (!pov.isValid()) { return DURATION_ZERO; }
    return m_povPressedDurations[pov.toIndex()];
}

wfl::int32_t DInputGamePad::analogInputValue(AnalogInput analog) const
{
    if (!analog.isValid()) { return 0; }
    return m_analogValues[analog.toIndex()];
}

bool DInputGamePad::initialize(
    const ComPtr<IDirectInputDevice8A>& device,
    HWND hWindow)
{
    if (!device || !hWindow) { return false; }

    m_device = device;

    m_hWindow = hWindow;

    return true;
}

void DInputGamePad::updateAllState(const Duration& deltaTime, const DIJOYSTATE2& joystate2)
{
    updateAnalogValues(joystate2);

    updateButtons(deltaTime, joystate2);

    updatePOVs(deltaTime, joystate2);
}

void DInputGamePad::updateAnalogValues(const DIJOYSTATE2& joystate2)
{
    m_analogValues[0] = static_cast<wfl::int32_t>(joystate2.lX);
    m_analogValues[1] = static_cast<wfl::int32_t>(joystate2.lY);
    m_analogValues[2] = static_cast<wfl::int32_t>(joystate2.lZ);
    m_analogValues[3] = static_cast<wfl::int32_t>(joystate2.lRx);
    m_analogValues[4] = static_cast<wfl::int32_t>(joystate2.lRy);
    m_analogValues[5] = static_cast<wfl::int32_t>(joystate2.lRz);
}

void DInputGamePad::updateButtons(const Duration& deltaTime, const DIJOYSTATE2& joystate2)
{
    for (wfl::size_t i = 0; i < GamePadButton::MAX_NUM; ++i)
    {
        m_buttonStateTables[1][i] = m_buttonStateTables[0][i];

        if (joystate2.rgbButtons[i] & 0x80)
        {
            m_buttonStateTables[0][i].set(InputStateType::Pressed);
            m_buttonPressedDurations[i] += deltaTime;
        }
        else
        {
            m_buttonStateTables[0][i].set(InputStateType::Released);
            m_buttonPressedDurations[i] = DURATION_ZERO;
        }
    }
}

void DInputGamePad::updatePOVs(const Duration& deltaTime, const DIJOYSTATE2& joystate2)
{
    constexpr wfl::array<wfl::uint8_t, POV::MAX_NUM> pov = {
        0x01, 0x09, 0x08, 0x0A, 0x02, 0x06, 0x04, 0x05, 0x00
    };

    int direction = (joystate2.rgdwPOV[0] + 2200) % 36000 / 4500;

    if (LOWORD(joystate2.rgdwPOV[0]) == 0xFFFF)
    {
        // 何も入力されていない状態
        direction = 8;
    }

    for (wfl::size_t i = 0; i < POV::MAX_NUM; ++i)
    {
        m_povStateTables[1][i] = m_povStateTables[0][i];

        if (pov[direction] & (0x01 << i))
        {
            m_povStateTables[0][i].set(InputStateType::Pressed);
            m_povPressedDurations[i] += deltaTime;
        }
        else
        {
            m_povStateTables[0][i].set(InputStateType::Released);
            m_povPressedDurations[i] = DURATION_ZERO;
        }
    }
}


} // namespace hid
} // namespace waffle

