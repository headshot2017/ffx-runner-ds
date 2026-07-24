#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <stdint.h>
#include <string>
#include <nds.h>

class CTexture
{
	int m_TextureID;

	GL_TEXTURE_TYPE_ENUM Palettize(int w, int h, int n, unsigned char* data, unsigned short* tmp, unsigned short* tmp_palette);

public:
	CTexture(std::string filename);
	~CTexture();

	void Bind();

	int Width();
	int Height();

	// used to create display lists
	uint32_t GetTextureFmt();
	uint16_t GetPaletteFmt();
};

#endif
