#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "entity.h"

class CMeshRenderer;

class CPlayer : public CEntity
{
public:
	CPlayer();
	~CPlayer();

	void CreateBody(b3WorldId worldId);
	void Update();
	void Render();

private:
	bool m_Fly;
	CMeshRenderer* m_pModel;

	b3BoxHull m_BoxHull;
};

#endif
