#include "VirtualJoystick.h"
#include "VirtualJoystickPrivate.h"


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


VirtualJoystick::VirtualJoystick(uint id, uint nbButtons, uint nbAxes, uint nbPovs, bool bForcedInit)
{
	pimpl = new VirtualJoystickPrivate{id,nbButtons,nbAxes,nbPovs,bForcedInit};
}

VirtualJoystick::~VirtualJoystick()
{
	delete pimpl;
}

bool VirtualJoystick::isVJoyDeviceFree(uint numDevice) {return pimpl->isVJoyDeviceFree(numDevice);}
bool VirtualJoystick::flush(bool bEvenIfNoChange) {return pimpl->flush(bEvenIfNoChange);}
void VirtualJoystick::resetReport() {pimpl->resetReport();}

bool VirtualJoystick::setButton(uint button, bool bPressed) {return pimpl->setButton(button,bPressed);}
bool VirtualJoystick::toggleButton(uint button) {return pimpl->toggleButton(button);}
bool VirtualJoystick::getButton(uint button) const {return pimpl->getButton(button);}

bool VirtualJoystick::setAxis(uint axis, float value) {return pimpl->setAxis(axis,value);}
float VirtualJoystick::getAxis(uint axis) const {return pimpl->getAxis(axis);}

bool VirtualJoystick::setPov(uint pov, float value) {return pimpl->setPov(pov,value);}
float VirtualJoystick::getPov(uint pov) const {return pimpl->getPov(pov);}

