#ifndef VIRTUAL_JOYSTICK_PRIVATE
#define VIRTUAL_JOYSTICK_PRIVATE


#include "../../3rdparty/vJoyModifiedInterface/stdafx.h"
#include "../../3rdparty/vJoyModifiedInterface/public.h"
using uint = unsigned int;


class VirtualJoystickPrivate
{
	public:
		VirtualJoystickPrivate(uint id, bool bForcedInit = true);
		VirtualJoystickPrivate(const VirtualJoystickPrivate &other) = delete;
		VirtualJoystickPrivate(VirtualJoystickPrivate &&other) = delete;
		VirtualJoystickPrivate& operator=(const VirtualJoystickPrivate &other) = delete;
		VirtualJoystickPrivate& operator=(VirtualJoystickPrivate &&other) = delete;
		~VirtualJoystickPrivate();
		
		uint id() const;
		void resetReport();
		bool flush(bool bEvenIfNoChange = false);
		bool isVJoyDeviceFree(uint numDevice);

		bool setButton(uint button, bool bPressed);
		bool toggleButton(uint button);
		bool getButton(uint button) const;
		
		bool setAxis(uint axis, float value);
		float getAxis(uint axis) const;
		
		bool setPov(uint pov, float value);
		float getPov(uint pov) const;
		
		
	private:
		LONG getAxisPrivate(uint axis) const;
		DWORD getPovPrivate(uint pov) const;
		
		uint m_id;
		bool m_reportModified;
		bool m_bUseDiscretePovs;
		JOYSTICK_POSITION_V2 m_report; // vJoy HID report
};

#endif
