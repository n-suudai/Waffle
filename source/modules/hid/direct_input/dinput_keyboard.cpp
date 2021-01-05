
#include "dinput_keyboard.h"
#include "dinput_utility.h"


namespace waffle {
namespace hid {


wfl::array<wfl::uint8_t, KeyCode::MAX_NUM> DInputKeyboard::s_keyMapTable = { 255 };

void DInputKeyboard::buildKeyMapTable()
{
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_A)] = DIK_A;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_B)] = DIK_B;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_C)] = DIK_C;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_D)] = DIK_D;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_E)] = DIK_E;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_F)] = DIK_F;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_G)] = DIK_G;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_H)] = DIK_H;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_I)] = DIK_I;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_J)] = DIK_J;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_K)] = DIK_K;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_L)] = DIK_L;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_M)] = DIK_M;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_N)] = DIK_N;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_O)] = DIK_O;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_P)] = DIK_P;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_Q)] = DIK_Q;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_R)] = DIK_R;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_S)] = DIK_S;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_T)] = DIK_T;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_U)] = DIK_U;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_V)] = DIK_V;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_W)] = DIK_W;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_X)] = DIK_X;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_Y)] = DIK_Y;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_Z)] = DIK_Z;

	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_1)] = DIK_1;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_2)] = DIK_2;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_3)] = DIK_3;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_4)] = DIK_4;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_5)] = DIK_5;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_6)] = DIK_6;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_7)] = DIK_7;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_8)] = DIK_8;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_9)] = DIK_9;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_0)] = DIK_0;

	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_F1)] = DIK_F1;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_F2)] = DIK_F2;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_F3)] = DIK_F3;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_F4)] = DIK_F4;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_F5)] = DIK_F5;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_F6)] = DIK_F6;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_F7)] = DIK_F7;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_F8)] = DIK_F8;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_F9)] = DIK_F9;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_F10)] = DIK_F10;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_F11)] = DIK_F11;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_F12)] = DIK_F12;

	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_Escape)] = DIK_ESCAPE;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_Return)] = DIK_RETURN;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_Space)] = DIK_SPACE;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_BackSpace)] = DIK_BACK;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_Delete)] = DIK_DELETE;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_Tab)] = DIK_TAB;

	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_LeftControl)] = DIK_LCONTROL;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_LeftAlt)] = DIK_LALT;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_LeftShift)] = DIK_LSHIFT;

	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_RightControl)] = DIK_RCONTROL;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_RightAlt)] = DIK_RALT;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_RightShift)] = DIK_RSHIFT;

	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_Up)] = DIK_UP;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_Down)] = DIK_DOWN;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_Left)] = DIK_LEFT;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_Right)] = DIK_RIGHT;

	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_Numpad1)] = DIK_NUMPAD1;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_Numpad2)] = DIK_NUMPAD2;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_Numpad3)] = DIK_NUMPAD3;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_Numpad4)] = DIK_NUMPAD4;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_Numpad5)] = DIK_NUMPAD5;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_Numpad6)] = DIK_NUMPAD6;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_Numpad7)] = DIK_NUMPAD7;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_Numpad8)] = DIK_NUMPAD8;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_Numpad9)] = DIK_NUMPAD9;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_Numpad0)] = DIK_NUMPAD0;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_NumpadEnter)] = DIK_NUMPADENTER;

	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_Minus)] = DIK_MINUS;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_Equals)] = DIK_EQUALS;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_Multiply)] = DIK_MULTIPLY;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_Subtract)] = DIK_SUBTRACT;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_Add)] = DIK_ADD;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_Divide)] = DIK_DIVIDE;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_Decimal)] = DIK_DECIMAL;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_Semicolon)] = DIK_SEMICOLON;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_Apostrophe)] = DIK_APOSTROPHE;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_Comma)] = DIK_COMMA;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_Period)] = DIK_PERIOD;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_Grave)] = DIK_GRAVE;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_Slash)] = DIK_SLASH;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_BackSlash)] = DIK_BACKSLASH;

	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_LeftBracket)] = DIK_LBRACKET;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_RightBracket)] = DIK_RBRACKET;

	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_PageUp)] = DIK_PGUP;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_PageDown)] = DIK_PGDN;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_End)] = DIK_END;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_SysRq)] = DIK_SYSRQ;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_Pause)] = DIK_PAUSE;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_Home)] = DIK_HOME;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_Capital)] = DIK_CAPITAL;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_NumLock)] = DIK_NUMLOCK;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_Scroll)] = DIK_SCROLL;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_Insert)] = DIK_INSERT;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_Apps)] = DIK_APPS;

	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_LeftWin)] = DIK_LWIN;
	s_keyMapTable[static_cast<wfl::size_t>(KeyCodeType::Key_RightWin)] = DIK_RWIN;
}

bool DInputKeyboard::createUnique(
	const ComPtr<IDirectInputDevice8A>& device,
	HWND hWindow,
	UniquePtr<IKeyboard>& outKeyboard)
{
	UniquePtr<DInputKeyboard> ptr = WFL_MAKE_UNIQUE(DInputKeyboard);

	if (ptr->initialize(device, hWindow))
	{
		outKeyboard = wfl::move(ptr);
		return true;
	}
	
	return false;
}

bool DInputKeyboard::createShared(
	const ComPtr<IDirectInputDevice8A>& device,
	HWND hWindow,
	SharedPtr<IKeyboard>& outKeyboard)
{
	SharedPtr<DInputKeyboard> ptr = WFL_MAKE_SHARED(DInputKeyboard);

	if (ptr->initialize(device, hWindow))
	{
		outKeyboard = wfl::move(ptr);
		return true;
	}

	return false;
}

DInputKeyboard::DInputKeyboard()
	: m_device()
	, m_hWindow(NULL)
	, m_stateTables()
	, m_pressedDurations()
{
	for (InputStateTable& table : m_stateTables)
	{
		table.fill(InputStateType::Released);
	}

	m_pressedDurations.fill(DURATION_ZERO);
}

DInputKeyboard::~DInputKeyboard()
{}

void DInputKeyboard::update(const Duration& deltaTime)
{
	if (!m_device) { return; }

	if (!DInputUtility::isWindowAvailable(m_hWindow)) { return; }
	
	if (!DInputUtility::DeviceAcquire(m_device)) { return; }

	if (!DInputUtility::DevicePoll(m_device)) { return; }	

	wfl::uint8_t keyBuffer[256];
	if (!DInputUtility::DeviceGetState(m_device, sizeof(keyBuffer), keyBuffer)) { return; }

	for (wfl::size_t i = 0; i < KeyCode::MAX_NUM; ++i)
	{
		m_stateTables[1][i] = m_stateTables[0][i];

		if (s_keyMapTable[i] != 255 && keyBuffer[s_keyMapTable[i]] & 0x80)
		{
			m_stateTables[0][i].set(InputStateType::Pressed);
			m_pressedDurations[i] += deltaTime;
		}
		else
		{
			m_stateTables[0][i].set(InputStateType::Released);
			m_pressedDurations[i] = DURATION_ZERO;
		}
	}
}

bool DInputKeyboard::isAnyPressed() const
{
	bool result = wfl::any_of(
		m_stateTables[0].begin(),
		m_stateTables[0].end(),
		[](const InputState& state) -> bool
		{
			return state.is(InputStateType::Pressed);
		});

	return result;
}

bool DInputKeyboard::isPressed(KeyCode key) const
{
	if (!key.isValid()) { return false; }
	return m_stateTables[0][key.toIndex()].is(InputStateType::Pressed);
}

bool DInputKeyboard::isReleased(KeyCode key) const
{
	if (!key.isValid()) { return false; }
	return m_stateTables[0][key.toIndex()].is(InputStateType::Released);
}

bool DInputKeyboard::isFirstPressed(KeyCode key) const
{
	if (!key.isValid()) { return false; }
	return m_stateTables[0][key.toIndex()].is(InputStateType::Pressed)
		&& m_stateTables[1][key.toIndex()].is(InputStateType::Released);
}

bool DInputKeyboard::isFirstReleased(KeyCode key) const
{
	if (!key.isValid()) { return false; }
	return m_stateTables[0][key.toIndex()].is(InputStateType::Released)
		&& m_stateTables[1][key.toIndex()].is(InputStateType::Pressed);
}

Duration DInputKeyboard::durationPressed(KeyCode key) const
{
	if (!key.isValid()) { return DURATION_ZERO; }
	return m_pressedDurations[key.toIndex()];
}

bool DInputKeyboard::initialize(
	const ComPtr<IDirectInputDevice8A>& device,
	HWND hWindow)
{
	if (!device || !hWindow) { return false; }

	m_device = device;

	m_hWindow = hWindow;

	return true;
}


} // namespace hid
} // namespace waffle

