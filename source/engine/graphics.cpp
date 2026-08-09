#include "graphics.h"
#include "texture.h"

CGraphics::~CGraphics()
{
	for (auto& pair : m_TextureRefs)
		delete pair.second.p;
	m_TextureRefs.clear();
}

CTexture* CGraphics::LoadTexture(std::string filename)
{
	if (m_TextureRefs.count(filename))
	{
		m_TextureRefs[filename].count++;
		return m_TextureRefs[filename].p;
	}

	CTexture* tex = LoadTextureImpl(filename);
	m_TextureRefs[filename] = {tex, 1};
	return tex;
}

void CGraphics::FreeTexture(CTexture* pHandle)
{
	for (auto& pair : m_TextureRefs)
	{
		if (pair.second.p != pHandle) continue;
		if (--pair.second.count <= 0)
		{
			delete pair.second.p;
			m_TextureRefs.erase(pair.first);
		}
		return;
	}
}
