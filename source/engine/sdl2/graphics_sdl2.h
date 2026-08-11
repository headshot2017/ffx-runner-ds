#ifndef GRAPHICS_SDL2_H_INCLUDED
#define GRAPHICS_SDL2_H_INCLUDED

#include <engine/graphics.h>

#include <SDL2/SDL.h>

class CGraphics_SDL2 : public CGraphics
{
public:
	CGraphics_SDL2();
	~CGraphics_SDL2();

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

private:
	SDL_Window* m_Window;
	SDL_GLContext m_Context;
};

#endif // GRAPHICS_SDL2_H_INCLUDED
