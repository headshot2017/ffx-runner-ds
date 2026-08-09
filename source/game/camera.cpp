#include "camera.h"

#include <string.h>

#include "entity.h"
#include <engine/engine.h>
#include <fixed.h>

CCamera::CCamera()
{
	m_pAttachedEntity = 0;
	m_Smooth = true;
	memset(m_Pos, 0, sizeof(m_Pos));
	memset(m_PosTarget, 0, sizeof(m_PosTarget));
	memset(m_PosLookAt, 0, sizeof(m_PosLookAt));
	memset(m_PosLookAtTarget, 0, sizeof(m_PosLookAtTarget));
}

CCamera::~CCamera()
{

}

void CCamera::AttachToEntity(CEntity* target)
{
	m_pAttachedEntity = target;
}

void CCamera::SnapToTarget()
{
	memcpy(m_Pos, m_PosTarget, sizeof(int)*3);
	memcpy(m_PosLookAt, m_PosLookAtTarget, sizeof(int)*3);
}

void CCamera::SetPos(int x, int y, int z)
{
	m_PosTarget[0] = x;
	m_PosTarget[1] = y;
	m_PosTarget[2] = z;
}

void CCamera::SetLookPos(int x, int y, int z)
{
	m_PosLookAtTarget[0] = x;
	m_PosLookAtTarget[1] = y;
	m_PosLookAtTarget[2] = z;
}

void CCamera::SetSmooth(bool on)
{
	m_Smooth = on;
}

void CCamera::Update()
{
	static int cameraSteps = 3;
	static int targetSteps = 2;

	if (m_pAttachedEntity)
	{
		int sine = sinLerp(m_pAttachedEntity->GetFaceAngle());
		int cosine = cosLerp(m_pAttachedEntity->GetFaceAngle());

		m_PosTarget[0] = m_pAttachedEntity->GetX() + (mulf32(96, sine));
		m_PosTarget[1] = m_pAttachedEntity->GetY() + (itof32(5) >> 9);
		m_PosTarget[2] = m_pAttachedEntity->GetZ() + (mulf32(96, cosine));

		m_PosLookAtTarget[0] = m_pAttachedEntity->GetX();
		m_PosLookAtTarget[1] = m_pAttachedEntity->GetY();
		m_PosLookAtTarget[2] = m_pAttachedEntity->GetZ();
	}

	if (m_Smooth)
	{
		int dX = (m_PosTarget[0] - m_Pos[0]) / cameraSteps;
		int dY = 0;
		int dZ = (m_PosTarget[2] - m_Pos[2]) / cameraSteps;

		m_Pos[0] = (dX == 0) ? m_PosTarget[0] : m_Pos[0]+dX;
		m_Pos[1] = (dY == 0) ? m_PosTarget[1] : m_Pos[1]+dY;
		m_Pos[2] = (dZ == 0) ? m_PosTarget[2] : m_Pos[2]+dZ;

		dX = (m_PosLookAtTarget[0] - m_PosLookAt[0]) / targetSteps;
		dY = (m_PosLookAtTarget[1] - m_PosLookAt[1]) / targetSteps;
		dZ = (m_PosLookAtTarget[2] - m_PosLookAt[2]) / targetSteps;

		m_PosLookAt[0] = (dX == 0) ? m_PosLookAtTarget[0] : m_PosLookAt[0]+dX;
		m_PosLookAt[1] = (dY == 0) ? m_PosLookAtTarget[1] : m_PosLookAt[1]+dY;
		m_PosLookAt[2] = (dZ == 0) ? m_PosLookAtTarget[2] : m_PosLookAt[2]+dZ;
	}
	else
	{
		SnapToTarget();
	}

	Engine().Graphics()->LookAt(
		m_Pos[0],       m_Pos[1],                          m_Pos[2],
		m_PosLookAt[0], m_PosLookAt[1]+(itof32(3) >> 9),   m_PosLookAt[2],
		0,              itof32(1),                         0
	);
}
