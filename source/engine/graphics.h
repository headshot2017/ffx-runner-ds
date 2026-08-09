#ifndef GRAPHICS_H_INCLUDED
#define GRAPHICS_H_INCLUDED

#include <unordered_map>
#include <cstdint>
#include <string>

class CTexture;

class CGraphics
{
public:
	enum
	{
		MAT_PROJECTION     = 0,
		MAT_POSITION       = 1,
		MAT_MODELVIEW      = 2,
		MAT_TEXTURE        = 3
	};

	virtual ~CGraphics();

	virtual void ClearColor(int r, int g, int b) = 0;
	virtual void Clear() = 0;

	virtual void Flush() = 0;

	virtual void SetClientState() = 0;

	virtual void MatrixMode(int mode) = 0;
	virtual void MatrixIdentity() = 0;
	virtual void PushMatrix() = 0;
	virtual void PopMatrix() = 0;
	virtual void LookAt(int x, int y, int z, int targetX, int targetY, int targetZ, int upX, int upY, int upZ) = 0;

	virtual void Scale(int x, int y, int z) = 0;
	virtual void Translate(int x, int y, int z) = 0;
	virtual void Rotate(int axis, int x, int y, int z) = 0;

	CTexture* LoadTexture(std::string filename);
	void FreeTexture(CTexture* pHandle);

protected:
	virtual CTexture* LoadTextureImpl(std::string filename) = 0;

private:
	struct TextureRef
	{
		CTexture* p;
		int count;
	};

	std::unordered_map<std::string, TextureRef> m_TextureRefs;
};

#endif // GRAPHICS_H_INCLUDED
