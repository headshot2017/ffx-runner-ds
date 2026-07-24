#ifndef __WORLD_H__
#define __WORLD_H__

#include <cstdint>
#include <string>

#include "box3d/box3d.h"

#define MAX_BODIES 128
#define MAX_JOINTS 1024
#define MAX_CONNECTIONS 2048

class CModel;
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

	CModel* m_pModel;
	CEntity* m_pFirstEntity;

	/*
	TPE_World m_tpeWorld;
	TPE_Body m_tpeBodies[MAX_BODIES];
	TPE_Joint m_tpeJoints[MAX_JOINTS];
	TPE_Connection m_tpeConnections[MAX_CONNECTIONS];
	unsigned int m_JointsUsed;
	unsigned int m_ConnectionsUsed;
	*/
	b3WorldId m_WorldId;
	b3BodyId m_WorldBody;

	int64_t m_TickCount;

private:
	//static uint8_t TPECollisionCallback(uint16_t b1, uint16_t j1, uint16_t b2, uint16_t j2, TPE_Vec3 point, void* pUserData);
	//static TPE_Vec3 TPEEnvironmentDistance(TPE_Vec3 p, TPE_Unit maxD, void* pUserData);
};

#endif
