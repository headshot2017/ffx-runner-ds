#include <nds.h>
#include <nds/arm9/videoGL.h>

#include "player.h"
#include "model.h"
#include "glext.h"


CPlayer::CPlayer() : CEntity()
{
	m_pModel = new CModel("models/player.obj");
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

	b3BodyDef groundBodyDef = b3DefaultBodyDef();
	b3ShapeDef groundShapeDef = b3DefaultShapeDef();
	b3BoxHull groundBox = b3MakeBoxHull(50.0f, 0.0f, 50.0f);
	groundBodyDef.position = (b3Vec3){ f32tofloat(m_X), -0.1f, f32tofloat(m_Z) };
	b3BodyId m_WorldBody = b3CreateBody(worldId, &groundBodyDef);
	b3CreateHullShape(m_WorldBody, &groundShapeDef, &groundBox.base);

	b3BodyDef bodyDef = b3DefaultBodyDef();
	bodyDef.type = b3_dynamicBody;
	bodyDef.position = (b3Vec3){ f32tofloat(m_X), f32tofloat(m_Y), f32tofloat(m_Z) };
	b3Vec3 axis = {0.0f, 0.0f, 1.0f};
	bodyDef.rotation = b3MakeQuatFromAxisAngle(axis, 30.f * B3_DEG_TO_RAD);
	m_BodyId = b3CreateBody(worldId, &bodyDef);

	m_BoxHull = b3MakeCubeHull(0.1f);
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

	b3Vec3 position = b3Body_GetPosition(m_BodyId);

	//printf("%.4f %.4f %.4f\n", position.x, position.y, position.z);
	m_X = floattof32(position.x);
	m_Y = floattof32(position.y);
	m_Z = floattof32(position.z);
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
		b3Quat rotation = b3Body_GetRotation(m_BodyId);

		float angle;
		b3Vec3 axis = b3GetAxisAngle(&angle, rotation);
		angle = (angle * B3_RAD_TO_DEG);
		const b3Vec3* v = b3GetHullPoints(&m_BoxHull.base);
		//for (int i=0; i<8; i++)
			//printf("%d: %.3f %.3f %.3f\n", i, v[i].x, v[i].y, v[i].z);

		//printf("%.4f %.4f %.4f\n", axis.x * angle, axis.y * angle, axis.z * angle);
		//m_FaceAngle = floattof32(axis.y * angle) << 3;
		glRotatef(angle, axis.x, axis.y, axis.z);
		/*
		carRot = TPE_bodyGetRotation(m_pBody,0,2,1);
		m_FaceAngle = (-carRot.y * 32768 / TPE_F) + 16384;
		//printf("a %d %d %d\n", carRot.x, carRot.y, carRot.z);
		glRotateZi(carRot.z * 32768 / TPE_F);
		glRotateXi(carRot.x * 32768 / TPE_F);
		glRotateYi(m_FaceAngle);
		*/
	}

	m_pModel->Render();

	glPopMatrix(1);
}
