#ifndef GRAPHICS_NDS_H_INCLUDED
#define GRAPHICS_NDS_H_INCLUDED

#include <engine/graphics.h>

class CGraphics_NDS : public CGraphics
{
public:
	CGraphics_NDS();
	~CGraphics_NDS();

	void ClearColor(int r, int g, int b);
	void Clear();

	void Flush();

	void SetClientState();

	void MatrixMode(int mode);
	void MatrixIdentity();
	void PushMatrix();
	void PopMatrix();
	void LookAt(int x, int y, int z, int targetX, int targetY, int targetZ, int upX, int upY, int upZ);

	void Scale(int x, int y, int z);
	void Translate(int x, int y, int z);
	void Rotate(int axis, int x, int y, int z);

protected:
	CTexture* LoadTextureImpl(std::string filename);
};

#endif // GRAPHICS_NDS_H_INCLUDED
