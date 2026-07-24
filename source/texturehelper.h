#ifndef __TEXTUREHELPER_H__
#define __TEXTUREHELPER_H__

#include <string>

class CTexture;

class CTextureHelper
{
public:
	static CTexture* Load(std::string filename);
	static void Free(CTexture* handle);
	static void Clear();
};

#endif