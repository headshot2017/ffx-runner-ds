#include <algorithm>

#include "inputcontroller.h"
#include "inputlistener.h"

void CInputController::Add(CInputListener* p)
{
	m_Listeners.push_back(p);
}

void CInputController::Remove(CInputListener* p)
{
	auto it = std::find(m_Listeners.begin(), m_Listeners.end(), p);
	if (it != m_Listeners.end())
		m_Listeners.erase(it);
}

void CInputController::Update()
{
	for (CInputListener*& i : m_Listeners)
		i->HandleInput();
}

CInputController& CInputController::ins()
{
	static CInputController s;
	return s;
}
