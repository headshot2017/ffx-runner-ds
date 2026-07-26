#include <math.h>

#include <nds.h>
#include <nds/arm9/videoGL.h>

#include "player.h"
#include "mesh/renderer.h"
#include "glext.h"


CPlayer::CPlayer() : CEntity()
{
	m_pModel = new CMeshRenderer("models/player.obj");
	m_FaceAngle = 0;
	m_Fly = false;
}

CPlayer::~CPlayer()
{
	delete m_pModel;
}

void CPlayer::CreateBody(b3WorldId worldId)
{
	printf("spawn %.4f %.4f %.4f\n", f32tofloat(m_X), f32tofloat(m_Y), f32tofloat(m_Z));

	// player body
	b3BodyDef bodyDef = b3DefaultBodyDef();
	bodyDef.type = b3_dynamicBody;
	bodyDef.position = (b3Vec3){ f32tofloat(m_X), f32tofloat(m_Y), f32tofloat(m_Z) };
	m_BodyId = b3CreateBody(worldId, &bodyDef);

	// player shape
	// to-do: don't hardcode this
	m_BoxHull = b3MakeBoxHull(0.00285f * SCALE_VERTICES, 0.00222f * SCALE_VERTICES, 0.0055f * SCALE_VERTICES);
	b3ShapeDef shapeDef = b3DefaultShapeDef();
	shapeDef.density = 1.0f;
	shapeDef.baseMaterial.friction = 0.3f;
	b3CreateHullShape(m_BodyId, &shapeDef, &m_BoxHull.base);

	//b3Body_SetAngularVelocity(m_BodyId, (b3Vec3){ -1, 0, 0 });
}

void CPlayer::Update()
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
		b3Body_SetLinearVelocity(m_BodyId, b3Vec3{0, 0, -8});
	if (keys & KEY_B)
		b3Body_SetLinearVelocity(m_BodyId, b3Vec3{0, 0, 8});
	if (keys & KEY_Y)
		b3Body_SetLinearVelocity(m_BodyId, b3Vec3{8, 0, 0});
	if (keys & KEY_X)
		b3Body_SetLinearVelocity(m_BodyId, b3Vec3{-8, 0, 0});

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

void CPlayer::Render()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glTranslatef32(m_X, m_Y, m_Z);

	if (m_Fly)
		glRotateYi(m_FaceAngle);
	else
	{
		float angle;
		b3Vec3 axis = b3GetAxisAngle(&angle, b3Body_GetRotation(m_BodyId));
		angle = (angle * B3_RAD_TO_DEG);

		glRotatef(angle, axis.x, axis.y, axis.z);
	}

	m_pModel->Render();

	glPopMatrix(1);
}
