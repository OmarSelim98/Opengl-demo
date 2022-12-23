#include <glad/glad.h>
#include "IndexBuffer.h"

void IndexBuffer::Destroy() const {
	glDeleteBuffers(1, &m_RendererId);
}

void IndexBuffer::Create(const void* data, unsigned int count)
{
	glGenBuffers(1, &m_RendererId);
	m_Count = count;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
}

void IndexBuffer::Bind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererId);

}
void IndexBuffer::UnBind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererId);
}