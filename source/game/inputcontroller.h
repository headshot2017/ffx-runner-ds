#ifndef INPUTCONTROLLER_H_INCLUDED
#define INPUTCONTROLLER_H_INCLUDED

#include <vector>

class CInputListener;

class CInputController
{
	std::vector<CInputListener*> m_Listeners;

public:
	void Add(CInputListener* p);
	void Remove(CInputListener* p);
	void Update();

	static CInputController& ins();
};

#endif // INPUTCONTROLLER_H_INCLUDED
