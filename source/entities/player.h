#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "car.h"

class CPlayer : public CCar
{
public:
	CPlayer();

	void CreateBody(b3WorldId worldId);
	void Update();

private:
	bool m_Fly;
};

#endif
