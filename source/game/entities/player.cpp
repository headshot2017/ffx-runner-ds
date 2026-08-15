#include "player.h"

#include <fixed.h>
#include <engine/engine.h>
#include <game/mesh/collider.h>
#include <game/mesh/renderer.h>

CPlayer::CPlayer(CWorld* world, int x, int y, int z) : CCar(world, CPlayer::TYPE, x, y, z), CInputListener()
{
	m_pModel = new CMeshRenderer("models/player.obj");
	m_Fly = false;

	m_Offset[1] = -0.0084f * SCALE_VERTICES;

	// player body
	b3BodyDef bodyDef = b3DefaultBodyDef();
	bodyDef.type = b3_dynamicBody;
	bodyDef.position = (b3Vec3){ f32tof(m_X), f32tof(m_Y), f32tof(m_Z) };
	m_BodyId = b3CreateBody(world->GetB3World(), &bodyDef);

	// player shape
	// to-do: don't hardcode this
	m_BoxHull = b3MakeBoxHull(0.00285f * SCALE_VERTICES, 0.00222f * SCALE_VERTICES, 0.0055f * SCALE_VERTICES);
	b3ShapeDef shapeDef = b3DefaultShapeDef();
	//shapeDef.density = 3.0f;
	shapeDef.baseMaterial.restitution = 0.0f;
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
	int keys = Engine().Input()->Held();

	if (m_Fly)
	{
		m_FaceAngle += (keys & CInput::LEFT) ? 512 : (keys & CInput::RIGHT) ? -512 : 0;
		m_Y += (keys & CInput::UP) ? 2 : (keys & CInput::DOWN) ? -2 : 0;
		if (keys & CInput::OK)
		{
			int sine = sinLerp(m_FaceAngle) >> 8;
			int cosine = cosLerp(m_FaceAngle) >> 8;

			m_X -= sine;
			m_Z -= cosine;
		}
		else if (keys & CInput::BACK)
		{
			int sine = sinLerp(m_FaceAngle) >> 8;
			int cosine = cosLerp(m_FaceAngle) >> 8;

			m_X += sine;
			m_Z += cosine;
		}
		printf("%.4f %.4f %.4f\n", f32tof(m_X), f32tof(m_Y), f32tof(m_Z));
		return;
	}

	if (!b3Body_IsValid(m_BodyId))
		return;

	printf("%.4f %.4f %.4f\n", f32tof(m_X), f32tof(m_Y), f32tof(m_Z));
	int controls[2] = {0};
	if (keys & CInput::OK)
	{
		++controls[0];
		b3Body_SetLinearVelocity(m_BodyId, b3Vec3{0, 0, -5});
	}
	if (keys & CInput::BACK)
	{
		--controls[0];
		b3Body_SetLinearVelocity(m_BodyId, b3Vec3{0, 0, 5});
	}
	if (keys & CInput::LEFT)
	{
		++controls[1];
		b3Body_SetLinearVelocity(m_BodyId, b3Vec3{5, 0, 0});
	}
	if (keys & CInput::RIGHT)
	{
		--controls[1];
		b3Body_SetLinearVelocity(m_BodyId, b3Vec3{-5, 0, 0});
	}
}

void CPlayer::Update()
{
	CCar::Update();
}
