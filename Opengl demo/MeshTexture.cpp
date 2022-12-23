#include "MeshTexture.h"
#include "MeshTexture.h"
#include "stb_image.h"
#include <glad/glad.h>


MeshTexture::MeshTexture(std::string fileName, unsigned int index, TEXTURE_TYPES type)
{
	m_Index = index;
	m_Type = type;
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load and generate the texture
	int width, height, nrChannels;
	unsigned char* data = stbi_load(fileName.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		GLenum format;
		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	
}

void MeshTexture::Destroy() const
{
	glDeleteTextures(1, &m_RendererID);
}

void MeshTexture::Bind()
{
	glActiveTexture(GL_TEXTURE0 + m_Index);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
}

void MeshTexture::UnBind()
{
	glActiveTexture(GL_TEXTURE0 + m_Index);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
}
