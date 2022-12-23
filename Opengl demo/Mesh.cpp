#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indicies, std::vector<Tex> textures)
{
	m_Vertices = vertices;
	m_Indicies = indicies;
	m_Textures = textures;

	m_VA.Create();
	m_VA.Bind();

	m_VB.Create(&vertices[0], vertices.size() * sizeof(Vertex));

	m_BufferLayout.AddElement<float>(3);
	m_BufferLayout.AddElement<float>(3);
	m_BufferLayout.AddElement<float>(2);

	m_IB.Create(&indicies[0], indicies.size());


	m_VA.AddVertexBuffer(m_VB, m_BufferLayout);


	m_VA.UnBind();
}

void Mesh::Destroy()
{
	m_IB.Destroy();
	m_VB.Destroy();
	m_VA.Destroy();
}