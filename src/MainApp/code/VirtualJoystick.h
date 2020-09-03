#ifndef VIRTUAL_JOYSTICK
#define VIRTUAL_JOYSTICK


#include <memory>
class VirtualJoystickPrivate;
using uint = unsigned int;


class VirtualJoystick
{
	public:
		VirtualJoystick(uint id, bool bForcedInit = true);
		VirtualJoystick(const VirtualJoystick &other) = delete;
		VirtualJoystick(VirtualJoystick &&other) = delete;
		VirtualJoystick& operator=(const VirtualJoystick &other) = delete;
		VirtualJoystick& operator=(VirtualJoystick &&other) = delete;
		~VirtualJoystick();
		
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
		std::unique_ptr<VirtualJoystickPrivate> pimpl;
};

#endif
