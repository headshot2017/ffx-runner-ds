#include <nds.h>
#include <nds/arm9/videoGL.h>

#include "player.h"
#include "mesh/collider.h"
#include "mesh/renderer.h"
#include "glext.h"

CPlayer::CPlayer(CWorld* world, int x, int y, int z) : CCar(world, CPlayer::TYPE, x, y, z), CInputListener()
{
	m_pModel = new CMeshRenderer("models/player.obj");
	m_Fly = false;

	m_Offset[1] = -0.0084f;

	// player body
	b3BodyDef bodyDef = b3DefaultBodyDef();
	bodyDef.type = b3_dynamicBody;
	bodyDef.position = (b3Vec3){ f32tofloat(m_X), f32tofloat(m_Y), f32tofloat(m_Z) };
	m_BodyId = b3CreateBody(world->GetB3World(), &bodyDef);

	// player shape
	// to-do: don't hardcode this
	m_BoxHull = b3MakeBoxHull(0.00285f * SCALE_VERTICES, 0.00222f * SCALE_VERTICES, 0.0055f * SCALE_VERTICES);
	b3ShapeDef shapeDef = b3DefaultShapeDef();
	shapeDef.density = 1.0f;
	shapeDef.baseMaterial.friction = 0.3f;
	b3CreateHullShape(m_BodyId, &shapeDef, &m_BoxHull.base);

	// Keep vehicle upright
	{
		b3ParallelJointDef parallelJointDef = b3DefaultParallelJointDef();
		parallelJointDef.base.bodyIdA = world->GetCollider()->GetBody();
		parallelJointDef.base.bodyIdB = m_BodyId;
		parallelJointDef.base.localFrameA.q = b3ComputeQuatBetweenUnitVectors( b3Vec3_axisZ, b3Vec3_axisY );
		parallelJointDef.base.localFrameB.q = b3ComputeQuatBetweenUnitVectors( b3Vec3_axisZ, b3Vec3_axisY );
		parallelJointDef.base.drawScale = 2.0f;
		parallelJointDef.base.collideConnected = true;
		parallelJointDef.hertz = 0.5f;
		parallelJointDef.dampingRatio = 1.0f;
		b3CreateParallelJoint( world->GetB3World(), &parallelJointDef );
	}
}

void CPlayer::HandleInput()
{
	u32 keys = keysHeld();

	if (m_Fly)
	{
		m_FaceAngle += (keys & KEY_LEFT) ? 512 : (keys & KEY_RIGHT) ? -512 : 0;
		m_Y += (keys & KEY_UP) ? 2 : (keys & KEY_DOWN) ? -2 : 0;
		if (keys & KEY_A)
		{
			int sine = sinLerp(m_FaceAngle) >> 8;
			int cosine = cosLerp(m_FaceAngle) >> 8;

			m_X -= sine;
			m_Z -= cosine;
		}
		else if (keys & KEY_B)
		{
			int sine = sinLerp(m_FaceAngle) >> 8;
			int cosine = cosLerp(m_FaceAngle) >> 8;

			m_X += sine;
			m_Z += cosine;
		}
		printf("%.4f %.4f %.4f\n", f32tofloat(m_X), f32tofloat(m_Y), f32tofloat(m_Z));
		return;
	}

	if (!b3Body_IsValid(m_BodyId))
		return;

	if (keys & KEY_A)
		b3Body_SetLinearVelocity(m_BodyId, b3Vec3{0, 0, -5});
	if (keys & KEY_B)
		b3Body_SetLinearVelocity(m_BodyId, b3Vec3{0, 0, 5});
	if (keys & KEY_Y)
		b3Body_SetLinearVelocity(m_BodyId, b3Vec3{5, 0, 0});
	if (keys & KEY_X)
		b3Body_SetLinearVelocity(m_BodyId, b3Vec3{-5, 0, 0});
}

void CPlayer::Update()
{
	CCar::Update();
}
