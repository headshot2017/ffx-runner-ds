#ifndef __WORLD_H__
#define __WORLD_H__

#include <cstdint>
#include <string>

#include "box3d/box3d.h"

class CMeshCollider;
class CMeshRenderer;
class CEntity;

class CWorld
{
public:
	CWorld(std::string modelFile);
	~CWorld();

	void AddEntity(CEntity* pEnt);
	CEntity* FirstEntity();
	CEntity* LastEntity();

	void Update();
	void Render();

private:
	void OnDestroy(CEntity* pEnt);

	CMeshRenderer* m_pModel;
	CMeshCollider* m_pCollider;
	CEntity* m_pFirstEntity;

	b3WorldId m_WorldId;
};

#endif
