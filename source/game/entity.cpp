#include "entity.h"

#include <fixed.h>
#include "world.h"

CEntity::CEntity(CWorld* world, int type)
{
	m_Type = type;
	m_pWorld = world;
	world->AddEntity(this);

	m_X = m_Y = m_Z = m_FaceAngle = 0;
	m_Destroy = false;
	m_pPrevEntity = m_pNextEntity = 0;
}

CEntity::~CEntity()
{
	if (b3Body_IsValid(m_BodyId))
	{
		b3DestroyBody(m_BodyId);
	}
}

void CEntity::Destroy()
{
	m_Destroy = true;
}

void CEntity::SetPos(int x, int y, int z)
{
	m_X = x;
	m_Y = y;
	m_Z = z;

	if (b3Body_IsValid(m_BodyId))
		b3Body_SetTransform(m_BodyId, b3Pos{f32tof(x), f32tof(y), f32tof(z)}, b3Body_GetRotation(m_BodyId));
}

void CEntity::SetFaceAngle(int f)
{
	m_FaceAngle = f;
}

int CEntity::GetX()
{
	return m_X;
}

int CEntity::GetY()
{
	return m_Y;
}

int CEntity::GetZ()
{
	return m_Z;
}

int CEntity::GetFaceAngle()
{
	return m_FaceAngle;
}
