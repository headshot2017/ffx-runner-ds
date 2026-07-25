#include "camera.h"
#include "entity.h"

#include <nds.h>

CCamera::CCamera()
{
	m_pAttachedEntity = 0;
	m_X = m_Y = m_Z = m_tX = m_tY = m_tZ = 0;
	m_Smooth = true;
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
	m_X = m_tX;
	m_Y = m_tY;
	m_Z = m_tZ;
}

void CCamera::SetPos(int x, int y, int z)
{
	m_tX = x;
	m_tY = y;
	m_tZ = z;
}

void CCamera::SetLookPos(int x, int y, int z)
{
	m_lX = x;
	m_lY = y;
	m_lZ = z;
}

void CCamera::SetSmooth(bool on)
{
	m_Smooth = on;
}

void CCamera::Update()
{
	if (m_pAttachedEntity)
	{
		int sine = sinLerp(m_pAttachedEntity->GetFaceAngle()) >> 7;
		int cosine = cosLerp(m_pAttachedEntity->GetFaceAngle()) >> 7;

		m_tX = m_pAttachedEntity->GetX() + sine;
		m_tY = m_pAttachedEntity->GetY() + 48;
		m_tZ = m_pAttachedEntity->GetZ() + cosine;

		m_lX = m_pAttachedEntity->GetX();
		m_lY = m_pAttachedEntity->GetY() + 32;
		m_lZ = m_pAttachedEntity->GetZ();
	}

	if (m_Smooth)
	{
		m_X += (m_tX - m_X) >> 3;
		m_Y += (m_tY - m_Y) >> 3;
		m_Z += (m_tZ - m_Z) >> 3;
	}
	else
	{
		m_X = m_tX;
		m_Y = m_tY;
		m_Z = m_tZ;
	}

	gluLookAtf32(
		m_X, m_Y, m_Z,              // Position
		m_lX, m_lY, m_lZ,           // Look at
		0, 4096, 0                  // Up
	);
}
