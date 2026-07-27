#include "inputlistener.h"
#include "inputcontroller.h"

CInputListener::CInputListener()
{
	CInputController::ins().Add(this);
}

CInputListener::~CInputListener()
{
	CInputController::ins().Remove(this);
}
