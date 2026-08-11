#ifndef TEXTURE_SDL2_H_INCLUDED
#define TEXTURE_SDL2_H_INCLUDED

#include <engine/texture.h>

#include <GL/glew.h>

class CTexture_SDL2 : public CTexture
{
	GLuint m_TextureID;

public:
	CTexture_SDL2(CGraphics* graphics, std::string filename);
	~CTexture_SDL2();

	void Bind();
};

#endif // TEXTURE_SDL2_H_INCLUDED
