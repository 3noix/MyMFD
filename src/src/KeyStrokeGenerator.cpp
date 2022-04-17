#include "KeyStrokeGenerator.h"
#include <QStringList>
#include <windows.h>


///////////////////////////////////////////////////////////////////////////////
//  CONSTRUCTEUR
//  KEYSTR 2 KEYCODE
//  GENERATE KEY STROKE
///////////////////////////////////////////////////////////////////////////////


// CONSTRUCTEUR ///////////////////////////////////////////////////////////////
KeyStrokeGenerator::KeyStrokeGenerator()
{
	// modifiers
	m_keystr2keycode_data["LShift"] = VK_LSHIFT;
	m_keystr2keycode_data["RShift"] = VK_RSHIFT;
	m_keystr2keycode_data["LCtrl"]  = VK_LCONTROL;
	m_keystr2keycode_data["LAlt"]   = VK_LMENU;
	
	// special keys
	m_keystr2keycode_data["Backspace"] = VK_BACK;
	m_keystr2keycode_data["Tab"]       = VK_TAB;
	m_keystr2keycode_data["Return"]    = VK_RETURN;
	m_keystr2keycode_data["Escape"]    = VK_ESCAPE;
	m_keystr2keycode_data["Space"]     = VK_SPACE;
	m_keystr2keycode_data["CapsLock"]  = VK_CAPITAL;
	
	// digits (the ones that are above letters)
	m_keystr2keycode_data["0"] = 0x30;
	m_keystr2keycode_data["1"] = 0x31;
	m_keystr2keycode_data["2"] = 0x32;
	m_keystr2keycode_data["3"] = 0x33;
	m_keystr2keycode_data["4"] = 0x34;
	m_keystr2keycode_data["5"] = 0x35;
	m_keystr2keycode_data["6"] = 0x36;
	m_keystr2keycode_data["7"] = 0x37;
	m_keystr2keycode_data["8"] = 0x38;
	m_keystr2keycode_data["9"] = 0x39;

	// digits (the ones from the numpad)
	m_keystr2keycode_data["Numpad0"] = VK_NUMPAD0;
	m_keystr2keycode_data["Numpad1"] = VK_NUMPAD1;
	m_keystr2keycode_data["Numpad2"] = VK_NUMPAD2;
	m_keystr2keycode_data["Numpad3"] = VK_NUMPAD3;
	m_keystr2keycode_data["Numpad4"] = VK_NUMPAD4;
	m_keystr2keycode_data["Numpad5"] = VK_NUMPAD5;
	m_keystr2keycode_data["Numpad6"] = VK_NUMPAD6;
	m_keystr2keycode_data["Numpad7"] = VK_NUMPAD7;
	m_keystr2keycode_data["Numpad8"] = VK_NUMPAD8;
	m_keystr2keycode_data["Numpad9"] = VK_NUMPAD9;
	m_keystr2keycode_data["NumpadMult"] = VK_MULTIPLY;
	m_keystr2keycode_data["NumpadAdd"]  = VK_ADD;
	m_keystr2keycode_data["NumpadSub"]  = VK_SUBTRACT;
	m_keystr2keycode_data["NumpadPt"]   = VK_DECIMAL;

	// letters
	m_keystr2keycode_data["A"] = 0x41;
	m_keystr2keycode_data["B"] = 0x42;
	m_keystr2keycode_data["C"] = 0x43;
	m_keystr2keycode_data["D"] = 0x44;
	m_keystr2keycode_data["E"] = 0x45;
	m_keystr2keycode_data["F"] = 0x46;
	m_keystr2keycode_data["G"] = 0x47;
	m_keystr2keycode_data["H"] = 0x48;
	m_keystr2keycode_data["I"] = 0x49;
	m_keystr2keycode_data["J"] = 0x4A;
	m_keystr2keycode_data["K"] = 0x4B;
	m_keystr2keycode_data["L"] = 0x4C;
	m_keystr2keycode_data["M"] = 0x4D;
	m_keystr2keycode_data["N"] = 0x4E;
	m_keystr2keycode_data["O"] = 0x4F;
	m_keystr2keycode_data["P"] = 0x50;
	m_keystr2keycode_data["Q"] = 0x51;
	m_keystr2keycode_data["R"] = 0x52;
	m_keystr2keycode_data["S"] = 0x53;
	m_keystr2keycode_data["T"] = 0x54;
	m_keystr2keycode_data["U"] = 0x55;
	m_keystr2keycode_data["V"] = 0x56;
	m_keystr2keycode_data["W"] = 0x57;
	m_keystr2keycode_data["X"] = 0x58;
	m_keystr2keycode_data["Y"] = 0x59;
	m_keystr2keycode_data["Z"] = 0x5A;
	
	// F keys
	m_keystr2keycode_data["F1"]  = VK_F1;
	m_keystr2keycode_data["F2"]  = VK_F2;
	m_keystr2keycode_data["F3"]  = VK_F3;
	m_keystr2keycode_data["F4"]  = VK_F4;
	m_keystr2keycode_data["F5"]  = VK_F5;
	m_keystr2keycode_data["F6"]  = VK_F6;
	m_keystr2keycode_data["F7"]  = VK_F7;
	m_keystr2keycode_data["F8"]  = VK_F8;
	m_keystr2keycode_data["F9"]  = VK_F9;
	m_keystr2keycode_data["F10"] = VK_F10;
	m_keystr2keycode_data["F11"] = VK_F11;
	m_keystr2keycode_data["F12"] = VK_F12;
	m_keystr2keycode_data["F13"] = VK_F13;
	m_keystr2keycode_data["F14"] = VK_F14;
	m_keystr2keycode_data["F15"] = VK_F15;
	m_keystr2keycode_data["F16"] = VK_F16;
	m_keystr2keycode_data["F17"] = VK_F17;
	m_keystr2keycode_data["F18"] = VK_F18;
	m_keystr2keycode_data["F19"] = VK_F19;
	m_keystr2keycode_data["F20"] = VK_F20;
	m_keystr2keycode_data["F21"] = VK_F21;
	m_keystr2keycode_data["F22"] = VK_F22;
	m_keystr2keycode_data["F23"] = VK_F23;
	m_keystr2keycode_data["F24"] = VK_F24;

	// WARNING, not validated:
	// keys that don't have the expected result on my setup
	m_keystr2keycode_data["PageUp"]     = VK_PRIOR;
	m_keystr2keycode_data["PageDown"]   = VK_NEXT;
	m_keystr2keycode_data["End"]        = VK_END;
	m_keystr2keycode_data["Home"]       = VK_HOME;
	m_keystr2keycode_data["Insert"]     = VK_INSERT;
	m_keystr2keycode_data["Delete"]     = VK_DELETE;
	m_keystr2keycode_data["ArrowLeft"]  = VK_LEFT;
	m_keystr2keycode_data["ArrowRight"] = VK_RIGHT;
	m_keystr2keycode_data["ArrowUp"]    = VK_UP;
	m_keystr2keycode_data["ArrowDown"]  = VK_DOWN;
	m_keystr2keycode_data["NumpadDiv"]  = VK_DIVIDE;
}

// KEYSTR 2 KEYCODE ///////////////////////////////////////////////////////////
KeyStrokeData KeyStrokeGenerator::keystr2keydata(const QString &keyStr)
{
	int nbPlus = keyStr.count('+');
	if (nbPlus > 1) {return {0,0};} // one modifier only!!
	
	if (nbPlus == 1)
	{
		// with a modifier
		QStringList list = keyStr.split('+',Qt::KeepEmptyParts);
		auto result1 = m_keystr2keycode_data.find(list[0]);
		auto result2 = m_keystr2keycode_data.find(list[1]);
		if (result1 == m_keystr2keycode_data.end()) {return {0,0};}
		if (result2 == m_keystr2keycode_data.end()) {return {0,0};}
		return {result1->second,result2->second};
	}
	
	// no modifier
	auto result = m_keystr2keycode_data.find(keyStr);
	if (result == m_keystr2keycode_data.end()) {return {0,0};}
	return {result->second,0};
}

// GENERATE KEY STROKE ////////////////////////////////////////////////////////
bool KeyStrokeGenerator::generateKeyStroke(const QString &keyStr, bool bDown)
{
	KeyStrokeData data = this->keystr2keydata(keyStr);
	if (data.key == 0) {return false;}
	this->generateKeyStroke(data.key,data.modifier,bDown);
	return true;
}

void KeyStrokeGenerator::generateKeyStroke(uint keycode, uint modifier, bool bDown)
{
	if (modifier > 0)
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
		
		if (bDown)
		{
			// a press event
			ips[0].ki.wScan = MapVirtualKey(keycode,MAPVK_VK_TO_VSC);
			ips[1].ki.wScan = MapVirtualKey(modifier,MAPVK_VK_TO_VSC);
			ips[0].ki.dwFlags = KEYEVENTF_SCANCODE;
			ips[1].ki.dwFlags = KEYEVENTF_SCANCODE;
		}
		else
		{
			// a release event
			ips[0].ki.wScan = MapVirtualKey(modifier,MAPVK_VK_TO_VSC);
			ips[1].ki.wScan = MapVirtualKey(keycode,MAPVK_VK_TO_VSC);
			ips[0].ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
			ips[1].ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
		}
		SendInput(2, ips, sizeof(INPUT));
	}
	else
	{
		INPUT ip;
		ip.type = INPUT_KEYBOARD;
		ip.ki.wVk = 0;
		ip.ki.time = 0;
		ip.ki.dwExtraInfo = 0;

		if (bDown)
		{
			// a press event
			ip.ki.wScan = MapVirtualKey(keycode,MAPVK_VK_TO_VSC);
			ip.ki.dwFlags = KEYEVENTF_SCANCODE;
		}
		else
		{
			// a release event
			ip.ki.wScan = MapVirtualKey(keycode,MAPVK_VK_TO_VSC);
			ip.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
		}
		SendInput(1, &ip, sizeof(INPUT));
	}
}

