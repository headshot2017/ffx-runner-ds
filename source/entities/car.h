#ifndef CAR_H_INCLUDED
#define CAR_H_INCLUDED

#include "entity.h"

class CMeshRenderer;

class CCar : public CEntity
{
public:
	CCar();
	~CCar();

	virtual void CreateBody(b3WorldId worldId) = 0;
	virtual void Update();
	void Render();

protected:
	CMeshRenderer* m_pModel;

	b3BoxHull m_BoxHull;
};

#endif // CAR_H_INCLUDED
