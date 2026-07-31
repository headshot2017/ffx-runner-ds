#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "world.h"

class CEntity
{
	friend class CWorld;

	int m_Type;

public:
	CEntity(CWorld* world, int type);
	virtual ~CEntity();

	int GetType() {return m_Type;}
	CWorld* GetWorld() {return m_pWorld;}

	virtual void Update() = 0;
	virtual void Render() = 0;

	void Destroy();
	void SetPos(int x, int y, int z);
	void SetFaceAngle(int f);
	int GetX();
	int GetY();
	int GetZ();
	int GetFaceAngle();

protected:
	int m_X;
	int m_Y;
	int m_Z;
	int m_FaceAngle;
	b3BodyId m_BodyId;
	CWorld* m_pWorld;

private:
	bool m_Destroy;
	CEntity* m_pPrevEntity;
	CEntity* m_pNextEntity;
};

#endif
