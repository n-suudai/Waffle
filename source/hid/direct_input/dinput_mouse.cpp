
#include "dinput_mouse.h"


namespace waffle {
namespace hid {


bool DInputMouse::createUnique(
	const ComPtr<IDirectInputDevice8A>& device,
	HWND hWindow,
	UniquePtr<IMouse>& outKeyboard)
{
	UniquePtr<DInputMouse> ptr = WFL_MAKE_UNIQUE(DInputMouse);

	if (ptr->initialize(device, hWindow))
	{
		outKeyboard = wfl::move(ptr);
		return true;
	}

	return false;
}

bool DInputMouse::createShared(
	const ComPtr<IDirectInputDevice8A>& device,
	HWND hWindow,
	SharedPtr<IMouse>& outKeyboard)
{
	SharedPtr<DInputMouse> ptr = WFL_MAKE_SHARED(DInputMouse);

	if (ptr->initialize(device, hWindow))
	{
		outKeyboard = wfl::move(ptr);
		return true;
	}

	return false;
}

DInputMouse::DInputMouse()
	: m_device()
	, m_hWindow(NULL)
	, m_stateTables()
	, m_pressedDurations()
	, m_axisValues()
{
	for (InputStateTable& table : m_stateTables)
	{
		table.fill(InputState(InputStateType::Released));
	}

	m_pressedDurations.fill(DURATION_ZERO);
	m_axisValues.fill(0);
}

DInputMouse::~DInputMouse()
{}

void DInputMouse::update(const Duration& deltaTime)
{
	if (!m_device) { return; }

	auto is_window_available = [](HWND hWindow) -> bool
	{
		HWND foreground = ::GetForegroundWindow();
		if (foreground != hWindow) { return false; }

		bool isWindowVisible = ::IsWindowVisible(hWindow) != 0;
		if (!isWindowVisible) { return false; }

		return true;
	};


	if (!is_window_available(m_hWindow)) { return; }

	auto acquire = [](const ComPtr<IDirectInputDevice8A>& device) -> bool
	{
		assert(!!device);

		HRESULT hr = device->Acquire();

		if (hr == DIERR_INVALIDPARAM) { return false; }
		if (hr == DIERR_NOTINITIALIZED) { return false; }
		if (hr == DIERR_OTHERAPPHASPRIO) { return false; }

		return true;
	};

	if (!acquire(m_device)) { return; }


	auto poll = [](const ComPtr<IDirectInputDevice8A>& device) -> bool
	{
		assert(!!device);

		HRESULT hr = device->Poll();

		if (hr == DIERR_INPUTLOST) { return false; }
		if (hr == DIERR_NOTACQUIRED) { return false; }
		if (hr == DIERR_NOTINITIALIZED) { return false; }

		return true;
	};

	if (!poll(m_device)) { return; }


	auto get_mouse_state = [](
		const ComPtr<IDirectInputDevice8A>& device,
		DIMOUSESTATE2& outMouseState)
	{
		HRESULT hr = device->GetDeviceState(sizeof(outMouseState), &outMouseState);

		if (hr == DIERR_INPUTLOST) { return false; }
		if (hr == DIERR_INVALIDPARAM) { return false; }
		if (hr == DIERR_NOTACQUIRED) { return false; }
		if (hr == DIERR_NOTINITIALIZED) { return false; }

		return true;
	};

	DIMOUSESTATE2 mousestate2 = { 0 };
	if (!get_mouse_state(m_device, mousestate2)) { return; }

	for (wfl::size_t i = 0; i < MouseButton::MAX_NUM; ++i)
	{
		m_stateTables[1][i] = m_stateTables[0][i];

		if (mousestate2.rgbButtons[i] & 0x80)
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

	wfl::array<LONG, MouseAxis::MAX_NUM> axisValues = {
		mousestate2.lX,
		mousestate2.lY,
		mousestate2.lZ
	};

	for (wfl::size_t i = 0; i < MouseAxis::MAX_NUM; ++i)
	{
		m_axisValues[i] = static_cast<wfl::int32_t>(axisValues[i]);
	}
}

bool DInputMouse::isAnyPressed() const
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

bool DInputMouse::isPressed(MouseButton button) const
{
	if (!button.isValid()) { return false; }
	return m_stateTables[0][button.toIndex()].is(InputStateType::Pressed);
}

bool DInputMouse::isReleased(MouseButton button) const
{
	if (!button.isValid()) { return false; }
	return m_stateTables[0][button.toIndex()].is(InputStateType::Released);
}

bool DInputMouse::isFirstPressed(MouseButton button) const
{
	if (!button.isValid()) { return false; }
	return m_stateTables[0][button.toIndex()].is(InputStateType::Pressed)
		&& m_stateTables[1][button.toIndex()].is(InputStateType::Released);
}

bool DInputMouse::isFirstReleased(MouseButton button) const
{
	if (!button.isValid()) { return false; }
	return m_stateTables[0][button.toIndex()].is(InputStateType::Released)
		&& m_stateTables[1][button.toIndex()].is(InputStateType::Pressed);
}

Duration DInputMouse::durationPressed(MouseButton button) const
{
	if (!button.isValid()) { return DURATION_ZERO; }
	return m_pressedDurations[button.toIndex()];
}

wfl::int32_t DInputMouse::axisValue(MouseAxis axis) const
{
	if (!axis.isValid()) { return 0; }
	return m_axisValues[axis.toIndex()];
}

bool DInputMouse::initialize(
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

