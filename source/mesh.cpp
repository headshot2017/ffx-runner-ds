#include "mesh.h"

#include <nds.h>

CMesh::CMesh(std::string filename)
{
	// load the obj
	m_pAttrib = new tinyobj::attrib_t;

	std::string warnings;
	std::string errors;
	if (!tinyobj::LoadObj(m_pAttrib, &m_Shapes, &m_Mats, &warnings, &errors, filename.c_str(), "models/"))
	{
		printf("WARN: %s\n", warnings.c_str());
		printf("ERR: %s\n", errors.c_str());
		while (1) swiWaitForVBlank();
	}
}

CMesh::~CMesh()
{
	Free();
}

void CMesh::Free()
{
	if (m_pAttrib != 0)
	{
		delete m_pAttrib;
		m_pAttrib = 0;
	}
	m_Shapes.clear();
	m_Mats.clear();
}
