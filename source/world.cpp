#include "world.h"

#include "mesh/collider.h"
#include "mesh/renderer.h"
#include "entity.h"
#include "inputcontroller.h"


CWorld::CWorld(std::string modelFile)
{
	b3WorldDef worldDef = b3DefaultWorldDef();
	worldDef.gravity = (b3Vec3){ 0.0f, -3.f, 0.0f };
	m_WorldId = b3CreateWorld(&worldDef);

	m_pModel = new CMeshRenderer(modelFile+".obj");
	m_pCollider = new CMeshCollider(modelFile+"_coll.obj", m_WorldId);

	m_pFirstEntity = 0;
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
	delete m_pCollider;
	b3DestroyWorld(m_WorldId);
}

void CWorld::AddEntity(CEntity* pEnt)
{
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
	CInputController::ins().Update();

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
