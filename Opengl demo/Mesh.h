#pragma once
#include <glm/glm.hpp>
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "MeshTexture.h"

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Tex {
    unsigned int id;
	std::string type;
	std::string path;
};  

class Mesh {
	private:
		std::vector<Vertex> m_Vertices;
		std::vector<unsigned int> m_Indicies;
		std::vector<Tex> m_Textures;
		VertexArray m_VA;
		VertexBuffer m_VB;
		VertexBufferLayout m_BufferLayout;
		IndexBuffer m_IB;
		unsigned int VBO, EBO;
	public:
		unsigned int VAO;
		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indicies, std::vector<Tex> textures);
		void Destroy();
		inline VertexArray VA() const { return m_VA; };
		inline std::vector<unsigned int> Indicies() const { return m_Indicies; };
		inline std::vector<Tex> Textures() const { return m_Textures; };
};