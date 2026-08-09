#include <game/mesh/renderer.h>

#include <fixed.h>
#include <engine/engine.h>
#include <engine/graphics.h>
#include <engine/texture.h>
#include <engine/nds/texture_nds.h>
#include <engine/nds/glext.h>

CMeshRenderer::CMeshRenderer(std::string filename, bool normals) : CMesh(filename)
{
	// load textures
	for (u32 i = 0; i < m_Mats.size(); i++)
	{
		tinyobj::material_t& mat = m_Mats[i];

		m_MaterialTextures[i] = Engine().Graphics()->LoadTexture("textures/" + mat.diffuse_texname);
		DC_FlushRange(m_MaterialTextures[i], sizeof(CTexture_NDS));
		printf("MAT %d: '%s' '%s' %d,%d\n", i, mat.name.c_str(), mat.diffuse_texname.c_str(), m_MaterialTextures[i]->Width(), m_MaterialTextures[i]->Height());
	}

	// calculate size for display list
	u32 totalSize = 2;
	int currentTexture = -1;
	for (u32 i = 0; i < m_Shapes.size(); i++)
	{
		tinyobj::shape_t &shape = m_Shapes[i];
		tinyobj::mesh_t &mesh = shape.mesh;
		printf("shape %d: %d %d %d\n", i, mesh.material_ids.size(), mesh.indices.size(), m_Mats.size());

		for (u32 j = 0; j < mesh.indices.size(); j++)
		{
			int texture = mesh.material_ids[j/3];
			if (texture != currentTexture)
			{
				currentTexture = texture;
				if (m_MaterialTextures.count(texture))
					totalSize += 3;
			}
		}

		totalSize += (mesh.indices.size() * (normals ? 5 : 4));
	}
	totalSize++;
	currentTexture = -1;

	uint32_t* list = new uint32_t[totalSize];
	list[0] = 0;

	uint32_t S = list[0];

	list[++S] = FIFO_COMMAND_PACK(FIFO_BEGIN, FIFO_NOP, FIFO_NOP, FIFO_NOP);
	list[++S] = GL_TRIANGLES;

	for (u32 i = 0; i < m_Shapes.size(); i++)
	{
		tinyobj::shape_t &shape = m_Shapes[i];
		tinyobj::mesh_t &mesh = shape.mesh;

		for (u32 j = 0; j < mesh.indices.size(); j++)
		{
			int texture = mesh.material_ids[j/3];
			if (texture != currentTexture)
			{
				currentTexture = texture;
				if (m_MaterialTextures.count(texture))
				{
					CTexture_NDS* tex = (CTexture_NDS*)(m_MaterialTextures[texture]);
					list[++S] = FIFO_COMMAND_PACK(FIFO_TEX_FORMAT, FIFO_PAL_FORMAT, FIFO_NOP, FIFO_NOP);
					list[++S] = tex->GetTextureFmt();
					list[++S] = tex->GetPaletteFmt();
				}
			}
			CTexture* tex = m_MaterialTextures[texture];

			tinyobj::index_t ind = mesh.indices[j];

			float u = m_pAttrib->texcoords[ind.texcoord_index * 2 + 0];
			float v = 1 - m_pAttrib->texcoords[ind.texcoord_index * 2 + 1];

			if (normals)
				list[++S] = FIFO_COMMAND_PACK(FIFO_TEX_COORD, FIFO_NORMAL, FIFO_VERTEX16, FIFO_NOP);
			else
				list[++S] = FIFO_COMMAND_PACK(FIFO_TEX_COORD, FIFO_VERTEX16, FIFO_NOP, FIFO_NOP);

			list[++S] = TEXTURE_PACK(floattot16(u * tex->Width()), floattot16(v * tex->Height()));
			if (normals)
				list[++S] = NORMAL_PACK(floattov10(m_pAttrib->normals[ind.normal_index * 3 + 0]), floattov10(m_pAttrib->normals[ind.normal_index * 3 + 1]), floattov10(m_pAttrib->normals[ind.normal_index * 3 + 2]));
			list[++S] = VERTEX_PACK(floattov16(m_pAttrib->vertices[ind.vertex_index * 3 + 0] / SCALE_VERTICES), floattov16(m_pAttrib->vertices[ind.vertex_index * 3 + 1] / SCALE_VERTICES));
			list[++S] = VERTEX_PACK(floattov16(m_pAttrib->vertices[ind.vertex_index * 3 + 2] / SCALE_VERTICES), 0);
		}
	}

	list[++S] = FIFO_COMMAND_PACK(FIFO_END, FIFO_NOP, FIFO_NOP, FIFO_NOP);
	list[0] = S;
	printf("final: %d %d\n", S, totalSize);

	Free();

	m_pDisplayList = list;
}

CMeshRenderer::~CMeshRenderer()
{
	delete[] ((uint32_t*)m_pDisplayList);
	for (auto& pair : m_MaterialTextures)
	{
		Engine().Graphics()->FreeTexture(pair.second);
	}
}

void CMeshRenderer::Render()
{
	glCallList(m_pDisplayList);
}
