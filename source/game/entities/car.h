#ifndef CAR_H_INCLUDED
#define CAR_H_INCLUDED

#include <game/entity.h>

class CMeshRenderer;

class CCar : public CEntity
{
public:
	CCar(CWorld* world, int type, int x, int y, int z);
	~CCar();

	virtual void Update();
	void Render();

protected:
	CMeshRenderer* m_pModel;
	int m_Offset[3];

	b3BoxHull m_BoxHull;
	b3JointId m_FrontLeftId;
	b3JointId m_FrontRightId;
	b3JointId m_RearLeftId;
	b3JointId m_RearRightId;
};

#endif // CAR_H_INCLUDED
