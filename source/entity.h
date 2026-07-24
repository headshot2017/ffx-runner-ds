#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "box3d/box3d.h"

class CEntity
{
	friend class CWorld;

public:
	CEntity();
	virtual ~CEntity();

	virtual void CreateBody(b3WorldId worldId) {}
	virtual void Update() {}
	virtual void Render() {}

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

private:
	bool m_Destroy;
	CEntity* m_pPrevEntity;
	CEntity* m_pNextEntity;
};

#endif
