#ifndef VIRTUAL_EVENT
#define VIRTUAL_EVENT


#include "ControlType.h"
class VirtualJoystick;
using uint = unsigned int;


// a structure that describe keyboard or virtual joysticks events
// such structures are sent to the VirtualEventsQueue that performs the events

enum class EventType
{
	Keyboard,
	VJoy
};


struct VJoyEvent
{
	VirtualJoystick* joystick;
	ControlType type; // button, axis or pov
	uint numButtonAxisPov;
	bool bButtonPressed;
	float axisOrPovValue;
};


struct KeyboardEvent
{
	bool bPress;
	uint key;
	uint modifier;
};


struct VirtualEvent
{
	EventType type;
	VJoyEvent vjev;
	KeyboardEvent vkev;
};


#endif


