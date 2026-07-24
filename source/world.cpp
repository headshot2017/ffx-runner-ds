#include "world.h"

#include "model.h"
#include "entity.h"


CWorld::CWorld(std::string modelFile)
{
	m_pModel = new CModel(modelFile);
	m_pFirstEntity = 0;

	/*
	TPE_worldInit(&m_tpeWorld, m_tpeBodies, 0, 0);
	m_tpeWorld.environmentFunction = TPEEnvironmentDistance;
	m_tpeWorld.collisionCallback = TPECollisionCallback;
	m_tpeWorld.userData = this;

	m_JointsUsed = 0;
	m_ConnectionsUsed = 0;
	*/

	b3WorldDef worldDef = b3DefaultWorldDef();
	worldDef.gravity = (b3Vec3){ 0.0f, -0.2f, 0.0f };
	m_WorldId = b3CreateWorld(&worldDef);

	m_TickCount = 0;
}

CWorld::~CWorld()
{
	for (CEntity* p = m_pFirstEntity; p;)
	{
		CEntity* next = p->m_pNextEntity;
		delete p;
		p = next;
	}
	delete m_pModel;
	//b3DestroyBody(m_WorldBody);
	b3DestroyWorld(m_WorldId);
}

void CWorld::AddEntity(CEntity* pEnt)
{
	/*
	int joints = 0, conns = 0;
	pEnt->m_BodyInd = m_tpeWorld.bodyCount;
	pEnt->m_pBody = &m_tpeBodies[m_tpeWorld.bodyCount];
	pEnt->MakeBody(&m_tpeJoints[m_JointsUsed], &m_tpeConnections[m_ConnectionsUsed], &joints, &conns);

	if (joints && conns)
	{
		m_JointsUsed += joints;
		m_ConnectionsUsed += conns;
		m_tpeWorld.bodyCount++;

		TPE_bodyMoveTo(pEnt->m_pBody, TPE_vec3(pEnt->m_X, pEnt->m_Y, pEnt->m_Z));
	}
	*/

	pEnt->CreateBody(m_WorldId);

	if (!m_pFirstEntity)
	{
		m_pFirstEntity = pEnt;
		return;
	}

	LastEntity()->m_pNextEntity = pEnt;
}

void CWorld::Update()
{
	static float timeStep = 1.0f / 60.0f;
	static int subStepCount = 4;

	b3World_Step(m_WorldId, timeStep, subStepCount);

	for (CEntity* p = m_pFirstEntity; p;)
	{
		while (p && p->m_Destroy)
		{
			CEntity* next = p->m_pNextEntity;
			OnDestroy(p);
			p = next;
		}
		if (!p) break;

		p->Update();
		p = p->m_pNextEntity;
	}
}

void CWorld::Render()
{
	m_pModel->Render();

	for (CEntity* p = m_pFirstEntity; p; p = p->m_pNextEntity)
		p->Render();
}

void CWorld::OnDestroy(CEntity* pEnt)
{
	if (pEnt->m_pPrevEntity)
	{
		pEnt->m_pPrevEntity->m_pNextEntity = pEnt->m_pNextEntity;
	}
	if (pEnt->m_pNextEntity)
	{
		pEnt->m_pNextEntity->m_pPrevEntity = pEnt->m_pPrevEntity;
	}
	if (pEnt == m_pFirstEntity)
	{
		m_pFirstEntity = 0;
	}
	delete pEnt;
}

CEntity* CWorld::FirstEntity()
{
	return m_pFirstEntity;
}

CEntity* CWorld::LastEntity()
{
	if (!m_pFirstEntity) return 0;

	CEntity* p = m_pFirstEntity;
	while(p)
	{
		if (!p->m_pNextEntity)
			break;
	}
	return p;
}
