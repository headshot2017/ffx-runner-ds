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
	shapeDef.density = 1.0f;
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

	shapeDef.density = 2.0f;
	shapeDef.baseMaterial.friction = 3.0f;

	bodyDef.type = b3_dynamicBody;
	bodyDef.allowFastRotation = true;
	bodyDef.rotation = b3ComputeQuatBetweenUnitVectors( b3Vec3_axisY, b3Vec3_axisZ );

	// b3HullData* hull = b3CreateCylinder( 0.25f, 0.4f, 0.0f, 16 );

	b3WheelJointDef jointDef = b3DefaultWheelJointDef();
	jointDef.base.bodyIdA = m_BodyId;
	jointDef.base.localFrameA.q = b3ComputeQuatBetweenUnitVectors( b3Vec3_axisX, b3Vec3_axisY );
	jointDef.base.localFrameB.q = b3ComputeQuatBetweenUnitVectors( b3Vec3_axisZ, b3Vec3_axisY );
	jointDef.enableSuspensionLimit = true;
	jointDef.lowerSuspensionLimit = -0.2f;
	jointDef.upperSuspensionLimit = 0.2f;
	jointDef.enableSuspensionSpring = true;
	jointDef.suspensionHertz = 4.0f;
	jointDef.suspensionDampingRatio = 0.7f;
	jointDef.enableSpinMotor = true;
	jointDef.maxSpinTorque = 5.0f;
	jointDef.enableSteering = true;
	jointDef.steeringHertz = 10.0f;
	jointDef.steeringDampingRatio = 0.7f;
	jointDef.targetSteeringAngle = 0.0f;
	jointDef.maxSteeringTorque = 5.0f;
	jointDef.enableSteeringLimit = true;
	jointDef.lowerSteeringLimit = B3_PI / 180.0f * -45.f;
	jointDef.upperSteeringLimit = B3_PI / 180.0f * 45.f;

	b3Sphere sphere = { b3Vec3_zero, 0.00125f };

	{
		bodyDef.position = { f32tof(m_X) - 0.06f * SCALE_VERTICES, f32tof(m_Y) - 0.045f * SCALE_VERTICES, f32tof(m_Z) + 0.135f * SCALE_VERTICES };
		b3BodyId bodyId = b3CreateBody( world->GetB3World(), &bodyDef );
		b3CreateSphereShape( bodyId, &shapeDef, &sphere );
		// b3CreateHullShape( bodyId, &shapeDef, hull );

		jointDef.base.bodyIdB = bodyId;
		jointDef.base.localFrameA.p = { -0.06f * SCALE_VERTICES, -0.1f * SCALE_VERTICES, 0.135f * SCALE_VERTICES };
		jointDef.enableSteering = true;
		jointDef.enableSpinMotor = false;
		m_FrontLeftId = b3CreateWheelJoint( world->GetB3World(), &jointDef );
	}

	{
		bodyDef.position = { f32tof(m_X) + 0.06f * SCALE_VERTICES, f32tof(m_Y) - 0.045f * SCALE_VERTICES, f32tof(m_Z) + 0.135f * SCALE_VERTICES };
		b3BodyId bodyId = b3CreateBody( world->GetB3World(), &bodyDef );
		b3CreateSphereShape( bodyId, &shapeDef, &sphere );
		// b3CreateHullShape( bodyId, &shapeDef, hull );

		jointDef.base.bodyIdB = bodyId;
		jointDef.base.localFrameA.p = { 0.06f * SCALE_VERTICES, -0.1f * SCALE_VERTICES, 0.135f * SCALE_VERTICES };
		jointDef.enableSteering = true;
		jointDef.enableSpinMotor = false;
		m_FrontRightId = b3CreateWheelJoint( world->GetB3World(), &jointDef );
	}

	{
		bodyDef.position = { f32tof(m_X) - 0.06f * SCALE_VERTICES, f32tof(m_Y) - 0.045f * SCALE_VERTICES, f32tof(m_Z) - 0.135f * SCALE_VERTICES };
		b3BodyId bodyId = b3CreateBody( world->GetB3World(), &bodyDef );
		b3CreateSphereShape( bodyId, &shapeDef, &sphere );
		// b3CreateHullShape( bodyId, &shapeDef, hull );

		jointDef.base.bodyIdB = bodyId;
		jointDef.base.localFrameA.p = { -0.06f * SCALE_VERTICES, -0.1f * SCALE_VERTICES, -0.135f * SCALE_VERTICES };
		jointDef.enableSteering = false;
		jointDef.enableSpinMotor = true;
		m_RearLeftId = b3CreateWheelJoint( world->GetB3World(), &jointDef );
	}

	{
		bodyDef.position = { f32tof(m_X) + 0.06f * SCALE_VERTICES, f32tof(m_Y) - 0.045f * SCALE_VERTICES, f32tof(m_Z) - 0.135f * SCALE_VERTICES };
		b3BodyId bodyId = b3CreateBody( world->GetB3World(), &bodyDef );
		b3CreateSphereShape( bodyId, &shapeDef, &sphere );
		// b3CreateHullShape( bodyId, &shapeDef, hull );

		jointDef.base.bodyIdB = bodyId;
		jointDef.base.localFrameA.p = { 0.06f * SCALE_VERTICES, -0.1f * SCALE_VERTICES, -0.135f * SCALE_VERTICES };
		jointDef.enableSteering = false;
		jointDef.enableSpinMotor = true;
		m_RearRightId = b3CreateWheelJoint( world->GetB3World(), &jointDef );
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
		b3Body_SetAwake( m_BodyId, true );
		++controls[0];
		//b3Body_SetLinearVelocity(m_BodyId, b3Vec3{0, 0, -5});
	}
	if (keys & CInput::BACK)
	{
		b3Body_SetAwake( m_BodyId, true );
		--controls[0];
		//b3Body_SetLinearVelocity(m_BodyId, b3Vec3{0, 0, 5});
	}
	if (keys & CInput::LEFT)
	{
		b3Body_SetAwake( m_BodyId, true );
		++controls[1];
	}
	if (keys & CInput::RIGHT)
	{
		b3Body_SetAwake( m_BodyId, true );
		--controls[1];
	}

	float maxSteeringAngle = 0.25f * B3_PI;
	b3WheelJoint_SetTargetSteeringAngle( m_FrontLeftId, maxSteeringAngle * controls[1] );
	b3WheelJoint_SetTargetSteeringAngle( m_FrontRightId, maxSteeringAngle * controls[1] );

	b3WheelJoint_SetSpinMotorSpeed( m_RearLeftId, 30 * controls[0] );
	b3WheelJoint_SetSpinMotorSpeed( m_RearRightId, 30 * controls[0] );
	/*
	if (keys & KEY_Y)
		b3Body_SetLinearVelocity(m_BodyId, b3Vec3{5, 0, 0});
	if (keys & KEY_X)
		b3Body_SetLinearVelocity(m_BodyId, b3Vec3{-5, 0, 0});
	*/
}

void CPlayer::Update()
{
	CCar::Update();
}
