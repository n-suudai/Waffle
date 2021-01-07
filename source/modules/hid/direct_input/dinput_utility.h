#pragma once

#include "direct_input.h"
#include "system/common/types.h"


namespace waffle {
namespace hid {


class DInputUtility
{
public:
	[[nodiscard]] static bool isWindowAvailable(HWND hWindow);

	[[nodiscard]] static bool DeviceAcquire(const ComPtr<IDirectInputDevice8A>& device);

	[[nodiscard]] static bool DevicePoll(const ComPtr<IDirectInputDevice8A>& device);

	[[nodiscard]] static bool DeviceGetState(
		const ComPtr<IDirectInputDevice8A>& device,
		wfl::size_t bytes,
		void* buffer);
};


} // namespace hid
} // namespace waffle

