#ifndef __CAMERA_H__
#define __CAMERA_H__

class CEntity;

class CCamera
{
public:
	CCamera();
	~CCamera();

	void AttachToEntity(CEntity* target);
	void SnapToTarget();
	void SetPos(int x, int y, int z);
	void SetLookPos(int x, int y, int z);
	void SetSmooth(bool on);

	int X() {return m_X;}
	int Y() {return m_Y;}
	int Z() {return m_Z;}

	void Update();

private:
	bool m_Smooth;
	CEntity* m_pAttachedEntity;
	int m_X;
	int m_Y;
	int m_Z;
	int m_tX;
	int m_tY;
	int m_tZ;
	int m_lX;
	int m_lY;
	int m_lZ;
};

#endif
