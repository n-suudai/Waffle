#include "dinput_utility.h"
#include <cassert>


namespace waffle {
namespace hid {


bool DInputUtility::isWindowAvailable(HWND hWindow)
{
	HWND foreground = ::GetForegroundWindow();
	if (foreground != hWindow) { return false; }

	bool isWindowVisible = ::IsWindowVisible(hWindow) != 0;
	if (!isWindowVisible) { return false; }

	return true;
}

bool DInputUtility::DeviceAcquire(const ComPtr<IDirectInputDevice8A>& device)
{
	assert(!!device);

	HRESULT hr = device->Acquire();

	if (hr == DIERR_INVALIDPARAM) { return false; }
	if (hr == DIERR_NOTINITIALIZED) { return false; }
	if (hr == DIERR_OTHERAPPHASPRIO) { return false; }

	return true;
}

bool DInputUtility::DevicePoll(const ComPtr<IDirectInputDevice8A>& device)
{
	assert(!!device);

	HRESULT hr = device->Poll();

	if (hr == DIERR_INPUTLOST) { return false; }
	if (hr == DIERR_NOTACQUIRED) { return false; }
	if (hr == DIERR_NOTINITIALIZED) { return false; }

	return true;
}

bool DInputUtility::DeviceGetState(
	const ComPtr<IDirectInputDevice8A>& device,
	wfl::size_t bytes,
	void* buffer)
{
	HRESULT hr = device->GetDeviceState(static_cast<DWORD>(bytes), buffer);

	if (hr == DIERR_INPUTLOST) { return false; }
	if (hr == DIERR_INVALIDPARAM) { return false; }
	if (hr == DIERR_NOTACQUIRED) { return false; }
	if (hr == DIERR_NOTINITIALIZED) { return false; }

	return true;
}


} // namespace hid
} // namespace waffle

