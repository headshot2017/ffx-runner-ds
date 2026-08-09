#include <game/entities/car.h>

#include <string.h>
#include <math.h>

#include <fixed.h>
#include <engine/engine.h>
#include <game/world.h>
#include <game/mesh/renderer.h>

CCar::CCar(CWorld* world, int type, int x, int y, int z) : CEntity(world, type)
{
	m_pModel = 0;
	m_X = x;
	m_Y = y;
	m_Z = z;
	memset(m_Offset, 0, sizeof(m_Offset));
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

	m_X = ftof32(position.x / SCALE_VERTICES);
	m_Y = ftof32(position.y / SCALE_VERTICES);
	m_Z = ftof32(position.z / SCALE_VERTICES);

	if (length >= 0.5f)
	{
		float ang = -atan2f(normalized.z, normalized.x);
		m_FaceAngle = static_cast<int>(ang * (DEGREES_IN_CIRCLE>>1) / B3_PI) - (DEGREES_IN_CIRCLE>>2);
	}
}

void CCar::Render()
{
	if (!m_pModel || !b3Body_IsValid(m_BodyId)) return;

	float angle;
	b3Vec3 axis = b3GetAxisAngle(&angle, b3Body_GetRotation(m_BodyId));
	angle = (angle * B3_RAD_TO_DEG);

	Engine().Graphics()->MatrixMode(CGraphics::MAT_MODELVIEW);
	Engine().Graphics()->PushMatrix();

	Engine().Graphics()->Translate(m_X+m_Offset[0], m_Y+m_Offset[1], m_Z+m_Offset[2]);
	Engine().Graphics()->Rotate(static_cast<int>(angle * DEGREES_IN_CIRCLE / 360.0f), ftof32(axis.x), ftof32(axis.y), ftof32(axis.z));

	m_pModel->Render();

	Engine().Graphics()->PopMatrix();
}
