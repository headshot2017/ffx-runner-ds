#ifndef COLLIDER_H_INCLUDED
#define COLLIDER_H_INCLUDED

#include <cstdint>
#include <unordered_map>

#include <box3d/box3d.h>

#include <game/mesh.h>

class CMeshCollider : public CMesh
{
	b3BodyId m_BodyId;
	b3MeshData* m_Mesh;

public:
	CMeshCollider(std::string filename, b3WorldId world);
	~CMeshCollider();

	b3BodyId GetBody() {return m_BodyId;}
};

#endif // COLLIDER_H_INCLUDED
