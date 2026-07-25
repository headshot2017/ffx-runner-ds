#ifndef __MESH_H__
#define __MESH_H__

#include <string>

#include "thirdparty/tiny_obj_loader.h"

class CMesh
{
protected:
	tinyobj::attrib_t* m_pAttrib;
	std::vector<tinyobj::shape_t> m_Shapes;
	std::vector<tinyobj::material_t> m_Mats;

public:
	CMesh(std::string filename);
	virtual ~CMesh();

	void Free();
};

#endif
