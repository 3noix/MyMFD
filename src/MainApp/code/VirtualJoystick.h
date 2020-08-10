#ifndef VIRTUAL_JOYSTICK
#define VIRTUAL_JOYSTICK


#include <QObject>
#include <QString>
#include <QColor>
#include "../../3rdparty/vJoyModifiedInterface/stdafx.h"
#include "../../3rdparty/vJoyModifiedInterface/public.h"
using uint = unsigned int;


class VirtualJoystick : public QObject
{
	Q_OBJECT
	
	public:
		VirtualJoystick(uint id, uint nbButtons = 128, uint nbAxes = 8, uint nbPovs = 0, bool bForcedInit = true);
		VirtualJoystick(const VirtualJoystick &other) = delete;
		VirtualJoystick(VirtualJoystick &&other) = delete;
		VirtualJoystick& operator=(const VirtualJoystick &other) = delete;
		VirtualJoystick& operator=(VirtualJoystick &&other) = delete;
		virtual ~VirtualJoystick();
		
		bool setButton(uint button, bool bPressed);
		bool toggleButton(uint button);
		bool getButton(uint button) const;
		bool setAxis(uint axis, float value);
		float getAxis(uint axis) const;
		bool setPov(uint pov, float value);
		float getPov(uint pov) const;
		
		void resetReport();
		bool flush(bool bEvenIfNoChange = false);
		bool isVJoyDeviceFree(uint numDevice);
		
		
	signals:
		void message(const QString &str, QColor color);
		
		
	private:
		LONG getAxisPrivate(uint axis) const;
		DWORD getPovPrivate(uint pov) const;
		
		uint m_id;
		bool m_reportModified;
		bool m_bUseDiscretePovs;
		JOYSTICK_POSITION_V2 m_report; // vJoy HID report
};

#endif
