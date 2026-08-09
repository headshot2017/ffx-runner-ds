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

	int* Pos() {return m_Pos;}

	void Update();

private:
	bool m_Smooth;
	CEntity* m_pAttachedEntity;

	int m_Pos[3];
	int m_PosTarget[3];

	int m_PosLookAt[3];
	int m_PosLookAtTarget[3];
};

#endif
