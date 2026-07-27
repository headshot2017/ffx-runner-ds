#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "car.h"
#include "../inputlistener.h"

class CPlayer : public CCar, public CInputListener
{
public:
	CPlayer();

	void CreateBody(b3WorldId worldId);
	void HandleInput();
	void Update();

private:
	bool m_Fly;
};

#endif
