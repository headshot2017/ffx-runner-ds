#include "texture.h"

#include <nds.h>

#include "glext.h"
#include "thirdparty/stb_image.h"


#define RGB_to_DS(src) \
	((src[0] & 0xF8) >> 3) | ((src[1] & 0xF8) << 2) | ((src[2] & 0xF8) << 7) | ((255 & 0x80) << 8)

#define RGBA8_to_DS(src) \
	((src[0] & 0xF8) >> 3) | ((src[1] & 0xF8) << 2) | ((src[2] & 0xF8) << 7) | ((src[3] & 0x80) << 8)

static int FindColorInPalette(u16* pal, int pal_size, u16 col)
{
	if ((col >> 15) == 0) return 0;
	
	for (int i = 1; i < pal_size; i++) {
		if(pal[i] == col) return i;
	}
	
	return -1;
}


CTexture::CTexture(std::string filename)
{
	m_TextureID = -1;

	int w, h, n;
	unsigned char *data = stbi_load(filename.c_str(), &w, &h, &n, 0);
	if (!data)
	{
		printf("FAILED stbi_load %s\n", filename.c_str());
		return;
	}

	// attempt to palettize texture
	u16* tmp = (u16*)malloc(w * h * sizeof(u16));
	if (!tmp)
	{
		printf("FAILED tmp\n");
		stbi_image_free(data);
		return;
	}
	u16* tmp_palette = (u16*)malloc(256*sizeof(u16));
	if (!tmp_palette)
	{
		printf("FAILED tmp_palette\n");
		stbi_image_free(data);
		free(tmp);
		return;
	}

	GL_TEXTURE_TYPE_ENUM GLformat = Palettize(w, h, n, data, tmp, tmp_palette);
	stbi_image_free(data);

	glGenTextures(1, &m_TextureID);
	glBindTexture(0, m_TextureID);

	if (glTexImage2D(GL_TEXTURE_2D, 0, GLformat, w, h, 0, 0, tmp) == 0)
	{
		glDeleteTextures(1, &m_TextureID);
		m_TextureID = -1;
		free(tmp);
		free(tmp_palette);
		printf("glTexImage2D failed %d %d %d '%s'\n", w, h, n, filename.c_str());
		return;
	}

	switch(GLformat)
	{
		case GL_RGB4:
			printf("%s GL_RGB4\n", filename.c_str());
			break;

		case GL_RGB16:
			printf("%s GL_RGB16\n", filename.c_str());
			break;

		case GL_RGB256:
			printf("%s GL_RGB256\n", filename.c_str());
			break;

		case GL_RGBA:
			printf("%s GL_RGBA\n", filename.c_str());
			break;

		case GL_RGB:
			printf("%s GL_RGB\n", filename.c_str());
			break;

		default:
			printf("%s default\n", filename.c_str());
			break;
	}

	if (GLformat != GL_RGBA && GLformat != GL_RGB)
	{
		int glPalSize;
		if(GLformat == GL_RGB4) glPalSize = 4;
		else if(GLformat == GL_RGB16) glPalSize = 16;
		else glPalSize = 256;

		glColorTableEXT(0, 0, glPalSize, 0, 0, tmp_palette);
	}
	
	glTexParameter(0, TEXGEN_TEXCOORD | GL_TEXTURE_WRAP_S | GL_TEXTURE_WRAP_T);

	free(tmp);
	free(tmp_palette);
}

CTexture::~CTexture()
{
	if (m_TextureID >= 0)
		glDeleteTextures(1, &m_TextureID);
}

GL_TEXTURE_TYPE_ENUM CTexture::Palettize(int w, int h, int n, unsigned char* data, unsigned short* tmp, unsigned short* tmp_palette)
{
	// transfer to tmp
	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			u8* src = data + ((y * w + x) * n);
			u16* dst = tmp + (y * w + x);

			if (n == 3)
				*dst = RGB_to_DS(src);
			else
				*dst = RGBA8_to_DS(src);
		}
	}

	tmp_palette[0] = 0;
	int pal_size = 1;
	for (int i = 0; i < w * h; i++)
	{
		u16 col = tmp[i];
	
		int idx = FindColorInPalette(tmp_palette, pal_size, col);
		
		if (idx == -1) {
			pal_size++;
			if (pal_size > 256) break;
			tmp_palette[pal_size - 1] = col;
		}
	}

	GL_TEXTURE_TYPE_ENUM GLformat = (n == 3) ? GL_RGB : GL_RGBA;
	if(pal_size <= 4) GLformat = GL_RGB4;
	else if(pal_size <= 16) GLformat = GL_RGB16;
	else if(pal_size <= 256) GLformat = GL_RGB256;

	if(GLformat != GL_RGBA && GLformat != GL_RGB)
	{
		char* tmp_chr = (char*) tmp;
		
		for (int i = 0; i < w * h; i++)
		{
			u16 col = tmp[i];
			int idx = FindColorInPalette(tmp_palette, pal_size, col);
			
			if(GLformat == GL_RGB256) {
				tmp_chr[i] = idx;
			} else if(GLformat == GL_RGB16) {
				if((i & 1) == 0) {
					tmp_chr[i >> 1] = idx;
				} else {
					tmp_chr[i >> 1] |= idx << 4;
				}
			} else {
				if((i & 3) == 0) {
					tmp_chr[i >> 2] = idx;
				} else {
					tmp_chr[i >> 2] |= idx << (2 * (i & 3));
				}
			}
		}
	}

	return GLformat;
}

void CTexture::Bind()
{
	if (m_TextureID <= 0) return;

	glBindTexture(0, m_TextureID);
}

int CTexture::Width()
{
	if (m_TextureID <= 0) return -1;

	gl_texture_data *tex = (gl_texture_data*)DynamicArrayGet( &glGlob.texturePtrs, m_TextureID );
	return 8 << ((tex->texFormat >> 20 ) & 7 ); // from videoGL.h glGetInt()
}

int CTexture::Height()
{
	if (m_TextureID <= 0) return -1;

	gl_texture_data *tex = (gl_texture_data*)DynamicArrayGet( &glGlob.texturePtrs, m_TextureID );
	return 8 << ((tex->texFormat >> 23 ) & 7 ); // from videoGL.h glGetInt()
}

uint32_t CTexture::GetTextureFmt()
{
	if (m_TextureID <= 0) return 0;

	gl_texture_data *tex = (gl_texture_data*)DynamicArrayGet( &glGlob.texturePtrs, m_TextureID );
	return tex->texFormat;
}

uint16_t CTexture::GetPaletteFmt()
{
	if (m_TextureID <= 0) return 0;

	gl_texture_data *tex = (gl_texture_data*)DynamicArrayGet( &glGlob.texturePtrs, m_TextureID );
	if (tex->palIndex)
	{
		gl_palette_data *pal = (gl_palette_data*)DynamicArrayGet( &glGlob.palettePtrs, tex->palIndex );
		return pal->addr;
	}
	return 0;
}
