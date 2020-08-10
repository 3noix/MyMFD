#include "VirtualEventsQueue.h"
#include "VirtualJoystick.h"
#include <windows.h>


///////////////////////////////////////////////////////////////////////////////
//  POST EVENT
//  POST EVENTS
//  PROCESS EVENTS
///////////////////////////////////////////////////////////////////////////////


// POST EVENT /////////////////////////////////////////////////////////////////
void VirtualEventsQueue::postEvent(const VirtualEvent &event)
{
	m_events.push_back(event);
}

// POST EVENTS ////////////////////////////////////////////////////////////////
void VirtualEventsQueue::postEvents(const std::vector<VirtualEvent> &events)
{
	m_events.reserve(m_events.size()+events.size());
	m_events.insert(m_events.end(), events.begin(), events.end());
}

// PROCESS EVENTS /////////////////////////////////////////////////////////////
void VirtualEventsQueue::processEvents()
{
	for (VirtualEvent &e : m_events)
	{
		if (e.type == EventType::VJoy && e.vjev.joystick) // it is a VJoyEvent and there is a VirtualJoystick specified
		{
			if (e.vjev.type == ControlType::Button)
			{
				e.vjev.joystick->setButton(e.vjev.numButtonAxisPov, e.vjev.bButtonPressed);
			}
			else if (e.vjev.type == ControlType::Axis)
			{
				e.vjev.joystick->setAxis(e.vjev.numButtonAxisPov, e.vjev.axisOrPovValue);
			}
			else if (e.vjev.type == ControlType::Pov)
			{
				e.vjev.joystick->setPov(e.vjev.numButtonAxisPov, e.vjev.axisOrPovValue);
			}
		}
		else if (e.type == EventType::Keyboard) // it is a keyboard event
		{
			if (e.vkev.modifier) // with a modifier
			{
				INPUT ips[2];
				ips[0].type = INPUT_KEYBOARD;
				ips[1].type = INPUT_KEYBOARD;
				ips[0].ki.wVk = 0;
				ips[1].ki.wVk = 0;
				ips[0].ki.time = 0;
				ips[1].ki.time = 0;
				ips[0].ki.dwExtraInfo = 0;
				ips[1].ki.dwExtraInfo = 0;
				
				if (e.vkev.bPress)
				{
					// a press event
					ips[0].ki.wScan = MapVirtualKey(e.vkev.modifier,MAPVK_VK_TO_VSC);
					ips[1].ki.wScan = MapVirtualKey(e.vkev.key,MAPVK_VK_TO_VSC);
					ips[0].ki.dwFlags = KEYEVENTF_SCANCODE;
					ips[1].ki.dwFlags = KEYEVENTF_SCANCODE;
				}
				else
				{
					// a release event
					ips[0].ki.wScan = MapVirtualKey(e.vkev.key,MAPVK_VK_TO_VSC);
					ips[1].ki.wScan = MapVirtualKey(e.vkev.modifier,MAPVK_VK_TO_VSC);
					ips[0].ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
					ips[1].ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
				}
				SendInput(2, ips, sizeof(INPUT));
			}
			else // with no modifier
			{
				INPUT ip;
				ip.type = INPUT_KEYBOARD;
				ip.ki.wVk = 0;
				ip.ki.time = 0;
				ip.ki.dwExtraInfo = 0;
				
				if (e.vkev.bPress)
				{
					// a press event
					ip.ki.wScan = MapVirtualKey(e.vkev.key,MAPVK_VK_TO_VSC);
					ip.ki.dwFlags = KEYEVENTF_SCANCODE;
				}
				else
				{
					// a release event
					ip.ki.wScan = MapVirtualKey(e.vkev.key,MAPVK_VK_TO_VSC);
					ip.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
				}
				SendInput(1, &ip, sizeof(INPUT));
			}
		}
	}
	
	m_events.clear();
}

