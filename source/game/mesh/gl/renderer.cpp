#include <game/mesh/renderer.h>

#include <fixed.h>
#include <engine/engine.h>
#include <engine/graphics.h>
#include <engine/texture.h>
#include <engine/sdl2/texture_sdl2.h>

struct MeshVec3
{
	float x;
	float y;
	float z;
};

struct MeshVec2
{
	float x;
	float y;
};

struct MeshData
{
	size_t size;
	MeshVec3* vertices;
	MeshVec3* normal;
	MeshVec2* uv;
};

CMeshRenderer::CMeshRenderer(std::string filename, bool normals) : CMesh(filename)
{
	// load textures
	for (size_t i = 0; i < m_Mats.size(); i++)
	{
		tinyobj::material_t& mat = m_Mats[i];

		m_MaterialTextures[i] = Engine().Graphics()->LoadTexture("textures/" + mat.diffuse_texname);
		printf("MAT %d: '%s' '%s' %d,%d\n", i, mat.name.c_str(), mat.diffuse_texname.c_str(), m_MaterialTextures[i]->Width(), m_MaterialTextures[i]->Height());
	}

	// calculate size for mesh data
	size_t totalSize = 0;
	for (size_t i = 0; i < m_Shapes.size(); i++)
	{
		tinyobj::shape_t &shape = m_Shapes[i];
		tinyobj::mesh_t &mesh = shape.mesh;
		printf("shape %d: %d %d %d\n", i, mesh.material_ids.size(), mesh.indices.size(), m_Mats.size());

		totalSize += mesh.indices.size();
	}

	printf("totalSize %d\n", totalSize);
	MeshData* data = new MeshData;
	data->vertices = new MeshVec3[totalSize];
	data->normal = (normals) ? new MeshVec3[totalSize] : 0;
	data->uv = new MeshVec2[totalSize];
	data->size = totalSize;

	size_t theInd = 0;
	for (size_t i = 0; i < m_Shapes.size(); i++)
	{
		tinyobj::shape_t &shape = m_Shapes[i];
		tinyobj::mesh_t &mesh = shape.mesh;

		for (size_t j = 0; j < mesh.indices.size(); j++)
		{
			tinyobj::index_t ind = mesh.indices[j];

			float u = m_pAttrib->texcoords[ind.texcoord_index * 2 + 0];
			float v = 1 - m_pAttrib->texcoords[ind.texcoord_index * 2 + 1];

			if (theInd >= totalSize)
				printf("theInd larger than totalSize! %d %d\n", theInd, totalSize);

			data->vertices[theInd].x = m_pAttrib->vertices[ind.vertex_index * 3 + 0] / SCALE_VERTICES;
			data->vertices[theInd].y = m_pAttrib->vertices[ind.vertex_index * 3 + 1] / SCALE_VERTICES;
			data->vertices[theInd].z = m_pAttrib->vertices[ind.vertex_index * 3 + 2] / SCALE_VERTICES;
			if (normals)
			{
				data->normal[theInd].x = m_pAttrib->vertices[ind.normal_index * 3 + 0];
				data->normal[theInd].y = m_pAttrib->vertices[ind.normal_index * 3 + 1];
				data->normal[theInd].z = m_pAttrib->vertices[ind.normal_index * 3 + 2];
			}
			data->uv[theInd].x = u;
			data->uv[theInd].y = v;

			++theInd;
		}
	}

	printf("final: %d %d\n", theInd, totalSize);

	Free();

	m_pDisplayList = data;
}

CMeshRenderer::~CMeshRenderer()
{
	delete[] ((MeshData*)m_pDisplayList);
	for (auto& pair : m_MaterialTextures)
	{
		Engine().Graphics()->FreeTexture(pair.second);
	}
}

void CMeshRenderer::Render()
{
	MeshData* data = reinterpret_cast<MeshData*>(m_pDisplayList);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, 0, data->vertices);
	glNormalPointer(GL_FLOAT, 0, data->normal);
	glTexCoordPointer(2, GL_FLOAT, 0, data->uv);

	glDrawArrays(GL_TRIANGLES, 0, data->size);
}
