#ifndef INPUTLISTENER_H_INCLUDED
#define INPUTLISTENER_H_INCLUDED

class CInputListener
{
public:
	CInputListener();
	~CInputListener();

	virtual void HandleInput() = 0;
};

#endif // INPUTLISTENER_H_INCLUDED
