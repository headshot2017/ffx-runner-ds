#include <math.h>

#include <nds.h>
#include <nds/arm9/videoGL.h>

#include "car.h"
#include "mesh/renderer.h"
#include "glext.h"

CCar::CCar() : CEntity()
{
	m_pModel = 0;
}

CCar::~CCar()
{
	if (m_pModel) delete m_pModel;
}

void CCar::Update()
{
	if (!b3Body_IsValid(m_BodyId))
		return;

	b3Vec3 position = b3Body_GetPosition(m_BodyId);
	b3Vec3 velocity = b3Body_GetLinearVelocity(m_BodyId);
	float length = b3Length(velocity);
	b3Vec3 normalized = b3Normalize(velocity);

	m_X = floattof32(position.x / SCALE_VERTICES);
	m_Y = floattof32(position.y / SCALE_VERTICES);
	m_Z = floattof32(position.z / SCALE_VERTICES);

	if (length >= 0.5f)
	{
		float ang = -atan2f(normalized.z, normalized.x);
		m_FaceAngle = (int)(ang * (DEGREES_IN_CIRCLE>>1) / B3_PI) - (DEGREES_IN_CIRCLE>>2);
	}
}

void CCar::Render()
{
	if (!m_pModel || !b3Body_IsValid(m_BodyId)) return;

	float angle;
	b3Vec3 axis = b3GetAxisAngle(&angle, b3Body_GetRotation(m_BodyId));
	angle = (angle * B3_RAD_TO_DEG);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glTranslatef32(m_X, m_Y, m_Z);
	glRotatef(angle, axis.x, axis.y, axis.z);

	m_pModel->Render();

	glPopMatrix(1);
}
