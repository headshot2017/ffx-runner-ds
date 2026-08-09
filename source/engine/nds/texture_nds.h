#ifndef TEXTURE_NDS_H_INCLUDED
#define TEXTURE_NDS_H_INCLUDED

#include <nds.h>
#include <engine/texture.h>

class CTexture_NDS : public CTexture
{
	int m_TextureID;

	GL_TEXTURE_TYPE_ENUM Palettize(int w, int h, int n, unsigned char* data, unsigned short* tmp, unsigned short* tmp_palette);
	int FindColorInPalette(uint16_t* pal, int pal_size, uint16_t col);

public:
	CTexture_NDS(CGraphics* graphics, std::string filename);
	~CTexture_NDS();

	void Bind();

	// used to create display lists
	uint32_t GetTextureFmt();
	uint16_t GetPaletteFmt();
};

#endif // TEXTURE_NDS_H_INCLUDED
