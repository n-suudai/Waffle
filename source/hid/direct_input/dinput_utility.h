#pragma once

#include "direct_input.h"
#include "common/types.h"


namespace waffle {
namespace hid {


class DInputUtility
{
public:
	static bool isWindowAvailable(HWND hWindow);

	static bool DeviceAcquire(const ComPtr<IDirectInputDevice8A>& device);

	static bool DevicePoll(const ComPtr<IDirectInputDevice8A>& device);

	static bool DeviceGetState(
		const ComPtr<IDirectInputDevice8A>& device,
		wfl::size_t bytes,
		void* buffer);
};


} // namespace hid
} // namespace waffle

