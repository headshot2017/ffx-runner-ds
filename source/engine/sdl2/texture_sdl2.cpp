#include "texture_sdl2.h"

#include "thirdparty/stb_image.h"


CTexture_SDL2::CTexture_SDL2(CGraphics* graphics, std::string filename) : CTexture(graphics, filename)
{
	m_TextureID = 0;

	int w, h, n;
	unsigned char *data = stbi_load(filename.c_str(), &w, &h, &n, 0);
	if (!data)
	{
		printf("FAILED stbi_load %s\n", filename.c_str());
		return;
	}

	glGenTextures(1, &m_TextureID);
	glBindTexture(GL_TEXTURE_2D, m_TextureID);

	glTexImage2D(GL_TEXTURE_2D, 0, (n == 3 ? GL_RGB : GL_RGBA), w, h, 0, (n == 3 ? GL_RGB : GL_RGBA), GL_UNSIGNED_BYTE, data);
	if (glGetError())
	{
		glDeleteTextures(1, &m_TextureID);
		m_TextureID = 0;
		printf("glTexImage2D failed %d %d %d '%s'\n", w, h, n, filename.c_str());
		return;
	}

	stbi_image_free(data);
	m_w = w;
	m_h = h;
}

CTexture_SDL2::~CTexture_SDL2()
{
	if (m_TextureID > 0)
		glDeleteTextures(1, &m_TextureID);
}

void CTexture_SDL2::Bind()
{
	if (m_TextureID <= 0) return;

	glBindTexture(0, m_TextureID);
}
