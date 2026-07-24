#ifndef __MODEL_H__
#define __MODEL_H__

#include <cstdint>
#include <string>
#include <unordered_map>

#include "thirdparty/tiny_obj_loader.h"

class CTexture;

class CModel
{
	uint32_t* m_pDisplayList;
	std::unordered_map<int, CTexture*> m_MaterialTextures;

	tinyobj::attrib_t* m_pAttrib;
	std::vector<tinyobj::shape_t> m_Shapes;
	std::vector<tinyobj::material_t> m_Mats;

public:
	CModel(std::string filename);
	~CModel();

	void Render();
};

#endif
