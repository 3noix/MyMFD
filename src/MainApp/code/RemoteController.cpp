#include "RemoteController.h"
#include "UjpsHttpServer.h"
#include "Lim.h"


///////////////////////////////////////////////////////////////////////////////
//  CONSTRUCTEUR
//  URL
//
//  BUTTONS COUNT
//  GET BUTTON
//  SET BUTTON
//
//  AXES COUNT
//  GET AXIS
//  SET AXIS
//
//  POVS COUNT
//  GET POV
//  SET POV
///////////////////////////////////////////////////////////////////////////////


// CONSTRUCTEUR ///////////////////////////////////////////////////////////////
RemoteController::RemoteController(const QString &name)
{
	m_name = name;
	
	m_nbButtons = 128;
	m_nbAxes = 8;
	m_nbPovs = 4;
	for (bool &b : m_buttons) {b = false;}
	for (float &f : m_axes) {f = 0.0f;}
	for (float &f : m_povs) {f = -1.0f;}
}

// URL ////////////////////////////////////////////////////////////////////////
QString RemoteController::url() const
{
	return UjpsHttpServer::instance().url(m_name);
}




// BUTTONS COUNT //////////////////////////////////////////////////////////////
uint RemoteController::buttonsCount() const
{
	return m_nbButtons;
}

// GET BUTTON /////////////////////////////////////////////////////////////////
bool RemoteController::getButton(uint button) const
{
	if (button >= m_nbButtons) {return false;}
	return m_buttons[button];
}

// SET BUTTON /////////////////////////////////////////////////////////////////
bool RemoteController::setButton(uint button, bool bPressed)
{
	if (button >= m_nbButtons) {return false;}
	m_buttons[button] = bPressed;
	return true;
}




// AXES COUNT /////////////////////////////////////////////////////////////////
uint RemoteController::axesCount() const
{
	return m_nbAxes;
}

// GET AXIS ///////////////////////////////////////////////////////////////////
float RemoteController::getAxis(uint axis) const
{
	if (axis >= m_nbAxes) {return 0.0f;}
	return m_axes[axis];
}

// SET AXIS ///////////////////////////////////////////////////////////////////
bool RemoteController::setAxis(uint axis, float value)
{
	if (axis >= m_nbAxes) {return false;}
	m_axes[axis] = value;
	return true;
}




// POVS COUNT /////////////////////////////////////////////////////////////////
uint RemoteController::povsCount() const
{
	return m_nbPovs;
}

// GET POV ////////////////////////////////////////////////////////////////////
float RemoteController::getPov(uint pov) const
{
	if (pov >= m_nbPovs) {return -1.0f;}
	return m_povs[pov];
}

// SET POV ////////////////////////////////////////////////////////////////////
bool RemoteController::setPov(uint pov, float value)
{
	if (pov >= m_nbPovs) {return false;}
	m_povs[pov] = value;
	return true;
}

