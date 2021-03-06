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
	m_keystr2keycode_data["LShift"] = 0xA0;
	m_keystr2keycode_data["RShift"] = 0xA1;
	m_keystr2keycode_data["LCtrl"]  = 0xA2;
	m_keystr2keycode_data["LAlt"]   = 0xA4;
	
	// special keys
	m_keystr2keycode_data["Backspace"] = 0x08;
	m_keystr2keycode_data["Tab"]       = 0x09;
	m_keystr2keycode_data["Return"]    = 0x0D;
	m_keystr2keycode_data["Escape"]    = 0x1B;
	m_keystr2keycode_data["Space"]     = 0x20;
	
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
	m_keystr2keycode_data["F1"]  = 0x70;
	m_keystr2keycode_data["F2"]  = 0x71;
	m_keystr2keycode_data["F3"]  = 0x72;
	m_keystr2keycode_data["F4"]  = 0x73;
	m_keystr2keycode_data["F5"]  = 0x74;
	m_keystr2keycode_data["F6"]  = 0x75;
	m_keystr2keycode_data["F7"]  = 0x76;
	m_keystr2keycode_data["F8"]  = 0x77;
	m_keystr2keycode_data["F9"]  = 0x78;
	m_keystr2keycode_data["F10"] = 0x79;
	m_keystr2keycode_data["F11"] = 0x7A;
	m_keystr2keycode_data["F12"] = 0x7B;
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

