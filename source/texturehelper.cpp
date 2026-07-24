#include "texturehelper.h"

#include <unordered_map>

#include "texture.h"

struct TextureRef
{
	CTexture* p;
	int count;
};

static std::unordered_map<std::string, TextureRef> m_Textures;

CTexture* CTextureHelper::Load(std::string filename)
{
	if (m_Textures.count(filename))
	{
		m_Textures[filename].count++;
		return m_Textures[filename].p;
	}

	CTexture* tex = new CTexture(filename);
	m_Textures[filename] = {tex, 1};
	return tex;
}

void CTextureHelper::Free(CTexture* handle)
{
	for (auto& pair : m_Textures)
	{
		if (pair.second.p != handle) continue;
		if (--pair.second.count <= 0)
		{
			delete pair.second.p;
			m_Textures.erase(pair.first);
		}
		return;
	}
}

void CTextureHelper::Clear()
{
	for (auto& pair : m_Textures)
		delete pair.second.p;
	m_Textures.clear();
}
