#include "VirtualJoystickPrivate.h"
#include <QString>
#include "Lim.h"
#include "ExceptionFailedToAcquireVJoyDevice.h"
#include "ExceptionBadVirtualJoystickArgs.h"
#include "../../3rdparty/vJoyModifiedInterface/vjoyinterface.h"

const int NB_JOYSTICKS_MAX = 16;


// to play bitwise with 32 bits words
// we use qint32 defined in QtGlobal or LONG to ensure sync with vJoy
LONG one = 1L;
LONG bitLong(uint i)
{
	//Q_ASSERT(i < 32);
	return (one << i%32);
};


///////////////////////////////////////////////////////////////////////////////
//  CONSTRUCTEUR
//  DESTRUCTEUR
//
//  IS VJOY DEVICE FREE
//  FLUSH
//  RESET REPORT
//
//  SET BUTTON
//  TOGGLE BUTTON
//  GET BUTTON
//  SET AXIS
//  GET AXIS
//  SET POV
//  GET POV
///////////////////////////////////////////////////////////////////////////////


// CONSTRUCTEUR ///////////////////////////////////////////////////////////////
VirtualJoystickPrivate::VirtualJoystickPrivate(uint id, uint nbButtons, uint nbAxes, uint nbPovs, bool bForcedInit)
{
	if (id > NB_JOYSTICKS_MAX || nbButtons > 128 || nbAxes > 8 || nbPovs > 4)
		throw ExceptionBadVirtualJoystickArgs{};
	
	m_id = id;
	m_reportModified = false;
	m_report.bDevice = (BYTE)m_id;
	
	// get status
	VjdStat status = GetVJDStatus(m_id);
	if (status == VJD_STAT_OWN)
		throw ExceptionFailedToAcquireVJoyDevice{"vJoy device " + QString::number(m_id).toStdString() + " is already owned by this feeder"};
	else if (status == VJD_STAT_BUSY)
		throw ExceptionFailedToAcquireVJoyDevice{"vJoy device " + QString::number(m_id).toStdString() + " is already owned by another feeder"};
	else if (status == VJD_STAT_MISS)
		throw ExceptionFailedToAcquireVJoyDevice{"vJoy device " + QString::number(m_id).toStdString() + " is not installed or disabled"};
	else if (status != VJD_STAT_FREE)
		throw ExceptionFailedToAcquireVJoyDevice{"vJoy device " + QString::number(m_id).toStdString() + " could not be acquired for an unknown reason"};
	
	// acquire device
	if (!AcquireVJD(m_id))
		throw ExceptionFailedToAcquireVJoyDevice{"Failed to acquire vJoy device " + QString::number(m_id).toStdString()};
	//ResetVJD(m_id);
	m_bUseDiscretePovs = (GetVJDDiscPovNumber(m_id) > 0);
	
	// init HID report
	this->resetReport();
	if (bForcedInit) {this->flush();}
}

// DESTRUCTEUR ////////////////////////////////////////////////////////////////
VirtualJoystickPrivate::~VirtualJoystickPrivate()
{
	RelinquishVJD(m_id);
}






// IS VJOY DEVICE FREE ////////////////////////////////////////////////////////
bool VirtualJoystickPrivate::isVJoyDeviceFree(uint numDevice)
{
	VjdStat status = GetVJDStatus(numDevice);
	return (status == VJD_STAT_FREE || status == VJD_STAT_MISS);
}

// FLUSH //////////////////////////////////////////////////////////////////////
bool VirtualJoystickPrivate::flush(bool bEvenIfNoChange)
{
	if (!m_reportModified && !bEvenIfNoChange) {return true;}
	bool b = UpdateVJD(m_id, &m_report);
	m_reportModified = false;
	return b;
}

// RESET REPORT ///////////////////////////////////////////////////////////////
void VirtualJoystickPrivate::resetReport()
{
	m_report.bDevice     = (BYTE)m_id;	// BYTE
	m_report.wThrottle   = 0L;			// LONG
	m_report.wRudder     = 0L;			// LONG
	m_report.wAileron    = 0L;			// LONG
	m_report.wAxisX      = 0x4000L;		// LONG
	m_report.wAxisY      = 0x4000L;		// LONG
	m_report.wAxisZ      = 0x4000L;		// LONG
	m_report.wAxisXRot   = 0x4000L;		// LONG
	m_report.wAxisYRot   = 0x4000L;		// LONG
	m_report.wAxisZRot   = 0x4000L;		// LONG
	m_report.wSlider     = 0x4000L;		// LONG
	m_report.wDial       = 0x4000L;		// LONG
	m_report.wWheel      = 0L;			// LONG
	m_report.wAxisVX     = 0L;			// LONG
	m_report.wAxisVY     = 0L;			// LONG
	m_report.wAxisVZ     = 0L;			// LONG
	m_report.wAxisVBRX   = 0L;			// LONG
	m_report.wAxisVBRY   = 0L;			// LONG
	m_report.wAxisVBRZ   = 0L;			// LONG
	m_report.lButtons    = 0L;			// LONG
	m_report.lButtonsEx1 = 0L;			// LONG
	m_report.lButtonsEx2 = 0L;			// LONG
	m_report.lButtonsEx3 = 0L;			// LONG
	m_report.bHats       = -1L;			// DWORD
	m_report.bHatsEx1    = -1L;			// DWORD
	m_report.bHatsEx2    = -1L;			// DWORD
	m_report.bHatsEx3    = -1L;			// DWORD
}






// SET BUTTON /////////////////////////////////////////////////////////////////
bool VirtualJoystickPrivate::setButton(uint button, bool bPressed)
{
	if (button >= 128) {return false;}
	
	if (button < 32 && bPressed)        {m_report.lButtons    |=  bitLong(button);}
	else if (button < 32 && !bPressed)  {m_report.lButtons    &= ~bitLong(button);}
	else if (button < 64 && bPressed)   {m_report.lButtonsEx1 |=  bitLong(button%32);}
	else if (button < 64 && !bPressed)  {m_report.lButtonsEx1 &= ~bitLong(button%32);}
	else if (button < 96 && bPressed)   {m_report.lButtonsEx2 |=  bitLong(button%32);}
	else if (button < 96 && !bPressed)  {m_report.lButtonsEx2 &= ~bitLong(button%32);}
	else if (button < 128 && bPressed)  {m_report.lButtonsEx3 |=  bitLong(button%32);}
	else if (button < 128 && !bPressed) {m_report.lButtonsEx3 &= ~bitLong(button%32);}
	
	m_reportModified = true;
	return true;
}

// TOGGLE BUTTON //////////////////////////////////////////////////////////////
bool VirtualJoystickPrivate::toggleButton(uint button)
{
	bool b = this->getButton(button);
	return this->setButton(button,!b);
}

// GET BUTTON /////////////////////////////////////////////////////////////////
bool VirtualJoystickPrivate::getButton(uint button) const
{
	if (button >= 128) {return false;}
	
	if (button < 32)       {return (m_report.lButtons    & bitLong(button));}
	else if (button < 64)  {return (m_report.lButtonsEx1 & bitLong(button%32));}
	else if (button < 96)  {return (m_report.lButtonsEx2 & bitLong(button%32));}
	else if (button < 128) {return (m_report.lButtonsEx3 & bitLong(button%32));}
	
	return false;
}

// SET AXIS ///////////////////////////////////////////////////////////////////
bool VirtualJoystickPrivate::setAxis(uint axis, float value)
{
	if (axis >= 8) {return false;}
	
	LONG v = lim<LONG>((value+1.0f)*16384.0f, 0L, 32768L);
	
	if (axis == 0)      {m_report.wAxisX    = v;}
	else if (axis == 1) {m_report.wAxisY    = v;}
	else if (axis == 2) {m_report.wAxisZ    = v;}
	else if (axis == 3) {m_report.wAxisXRot = v;}
	else if (axis == 4) {m_report.wAxisYRot = v;}
	else if (axis == 5) {m_report.wAxisZRot = v;}
	else if (axis == 6) {m_report.wSlider   = v;}
	else if (axis == 7) {m_report.wDial     = v;}
	
	m_reportModified = true;
	return true;
}

// GET AXIS ///////////////////////////////////////////////////////////////////
float VirtualJoystickPrivate::getAxis(uint axis) const
{
	if (axis >= 8) {return 0.0f;}
	LONG value2 = this->getAxisPrivate(axis);
	float value = (2.0f*value2/32768.0f) - 1.0f;
	return value;
}

LONG VirtualJoystickPrivate::getAxisPrivate(uint axis) const
{
	if (axis >= 8) {return 16384L;}
	
	if (axis == 0)      {return m_report.wAxisX;}
	else if (axis == 1) {return m_report.wAxisY;}
	else if (axis == 2) {return m_report.wAxisZ;}
	else if (axis == 3) {return m_report.wAxisXRot;}
	else if (axis == 4) {return m_report.wAxisYRot;}
	else if (axis == 5) {return m_report.wAxisZRot;}
	else if (axis == 6) {return m_report.wSlider;}
	else if (axis == 7) {return m_report.wDial;}
	
	return 16384L;
}

// SET POV ////////////////////////////////////////////////////////////////////
bool VirtualJoystickPrivate::setPov(uint pov, float value)
{
	if (pov >= 4) {return false;}
	
	if (!m_bUseDiscretePovs)
	{
		DWORD v = -1u;
		if (value >= 0.0f && value < 360.0f)
		{
			v = (DWORD) (value*100.0f);
			v = lim<DWORD>(v,0u,35999u);
		}
		
		if (pov == 0)      {m_report.bHats    = v;}
		else if (pov == 1) {m_report.bHatsEx1 = v;}
		else if (pov == 2) {m_report.bHatsEx2 = v;}
		else if (pov == 3) {m_report.bHatsEx3 = v;}
	}
	else
	{
		DWORD position = -1u;
		if (value >= 0.0f && value < 360.0f)
		{
			if (value < 60.0f  || value > 300.0f)      {position = 0;}
			else if (value > 30.0f  && value < 150.0f) {position = 1;}
			else if (value > 120.0f && value < 240.0f) {position = 2;}
			else if (value > 210.0f && value < 330.0f) {position = 3;}
		}
		
		position = (position<<(4*pov));
		DWORD mask = (0x0000000F<<(4*pov));
		DWORD v = m_report.bHats;
		v = ((~mask)&v) | (mask&(position));
		m_report.bHats = v;
	}
	
	m_reportModified = true;
	return true;
}

// GET POV ////////////////////////////////////////////////////////////////////
float VirtualJoystickPrivate::getPov(uint pov) const
{
	if (pov >= 4) {return -1.0f;}
	
	if (!m_bUseDiscretePovs)
	{
		DWORD value2 = this->getPovPrivate(pov);
		if (value2 == -1UL) {return -1.0f;}
		float value = 0.01f * value2;
		return value;
	}
	else
	{
		DWORD mask = (0x0000000F<<(4*pov));
		DWORD position = (mask & m_report.bHats) >> (4*pov);
		if (position < 4) {return 90.0f * position;}
		return -1.0f;
	}
}

DWORD VirtualJoystickPrivate::getPovPrivate(uint pov) const
{
	if (pov == 0)      {return m_report.bHats;}
	else if (pov == 1) {return m_report.bHatsEx1;}
	else if (pov == 2) {return m_report.bHatsEx2;}
	else if (pov == 3) {return m_report.bHatsEx3;}
	
	return -1;
}

