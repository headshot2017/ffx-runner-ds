#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "car.h"
#include <game/inputlistener.h>

class CPlayer : public CCar, public CInputListener
{
public:
	CPlayer(CWorld* world, int x, int y, int z);

	static constexpr int TYPE = 1;

	void CreateBody(b3WorldId worldId);
	void HandleInput();
	void Update();

private:
	bool m_Fly;
};

#endif
