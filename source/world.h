#ifndef __WORLD_H__
#define __WORLD_H__

#include <cstdint>
#include <string>

#include "box3d/box3d.h"

class CMeshCollider;
class CMeshRenderer;
class CEntity;
class CCamera;

class CWorld
{
public:
	CWorld(std::string modelFile, std::string skyFile);
	~CWorld();

	void AddEntity(CEntity* pEnt);
	CEntity* FirstEntity();
	CEntity* LastEntity();
	CEntity* FindEntity(int type, CEntity* start=0);

	b3WorldId GetB3World() {return m_WorldId;}
	CCamera* GetCamera() {return m_pCamera;}

	void Update();
	void Render();

private:
	void OnDestroy(CEntity* pEnt);

	CMeshRenderer* m_pModel;
	CMeshCollider* m_pCollider;
	CEntity* m_pFirstEntity;
	CCamera* m_pCamera;

	b3WorldId m_WorldId;
};

#endif
