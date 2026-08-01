#ifndef MESHRENDERER_H_INCLUDED
#define MESHRENDERER_H_INCLUDED

#include <cstdint>
#include <unordered_map>

#include "../mesh.h"

class CTexture;

class CMeshRenderer : public CMesh
{
	uint32_t* m_pDisplayList;
	std::unordered_map<int, CTexture*> m_MaterialTextures;

public:
	CMeshRenderer(std::string filename, bool normals=true);
	~CMeshRenderer();

	void Render();
};

#endif // MESHRENDERER_H_INCLUDED
