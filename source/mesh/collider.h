#ifndef COLLIDER_H_INCLUDED
#define COLLIDER_H_INCLUDED

#include <cstdint>
#include <unordered_map>

#include "../mesh.h"
#include "box3d/box3d.h"

class CMeshCollider : public CMesh
{
	b3BodyId m_BodyId;
	b3MeshData* m_Mesh;

public:
	CMeshCollider(std::string filename, b3WorldId world);
	~CMeshCollider();
};

#endif // COLLIDER_H_INCLUDED
