#include "collider.h"

struct TempMesh
{
	std::vector<b3Vec3> vertices;
	std::vector<int32_t> indices;
};

CMeshCollider::CMeshCollider(std::string filename, b3WorldId world) : CMesh(filename)
{
	TempMesh mesh;

	int vertexCount = int( m_pAttrib->vertices.size() / 3 );
	for ( int i = 0; i < vertexCount; i++ )
	{
		float x = m_pAttrib->vertices[3 * i + 0];
		float y = m_pAttrib->vertices[3 * i + 1];
		float z = m_pAttrib->vertices[3 * i + 2];
		b3Vec3 vertex = b3Vec3{ x, y, z };
		mesh.vertices.push_back( vertex );
	}

	// Loop over shapes
	int shapeCount = (int)m_Shapes.size();
	for ( int shapeIndex = 0; shapeIndex < shapeCount; ++shapeIndex )
	{
		int faceCount = (int)m_Shapes[shapeIndex].mesh.num_face_vertices.size();
		int baseIndex = 0;
		for ( int faceIndex = 0; faceIndex < faceCount; ++faceIndex )
		{
			int faceVertexCount = (int)m_Shapes[shapeIndex].mesh.num_face_vertices[faceIndex];

			// todo only triangles for now
			if ( faceVertexCount != 3 )
			{
				baseIndex += faceVertexCount;
				continue;
			}

			for ( int vertexIndex = 0; vertexIndex < faceVertexCount; ++vertexIndex )
			{
				tinyobj::index_t idx = m_Shapes[shapeIndex].mesh.indices[baseIndex + vertexIndex];
				mesh.indices.push_back( idx.vertex_index );
			}

			baseIndex += faceVertexCount;
		}
	}

	Free();

	b3BodyDef bodyDef = b3DefaultBodyDef();
	m_BodyId = b3CreateBody(world, &bodyDef);

	b3MeshDef def = {0};
	def.vertices       = mesh.vertices.data();
	def.vertexCount    = (int)mesh.vertices.size();
	def.indices        = mesh.indices.data();
	def.triangleCount  = (int)mesh.indices.size() / 3;
	def.useMedianSplit = false;
	def.weldVertices   = true;
	def.weldTolerance  = 0.002f;
	def.identifyEdges  = true;
	m_Mesh = b3CreateMesh(&def, NULL, 0);
	if (m_Mesh == 0)
		printf("failed to create mesh... %d %d\n", def.vertexCount, def.triangleCount);
	else
		printf("mesh created. %d %d\n", def.vertexCount, def.triangleCount);

	b3ShapeDef shapeDef = b3DefaultShapeDef();
	b3CreateMeshShape(m_BodyId, &shapeDef, m_Mesh, b3Vec3_one);
}

CMeshCollider::~CMeshCollider()
{
	if (b3Body_IsValid(m_BodyId))
	{
		b3DestroyBody(m_BodyId);
		b3DestroyMesh(m_Mesh);
	}
}
