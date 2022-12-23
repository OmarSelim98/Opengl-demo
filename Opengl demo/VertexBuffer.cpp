#include <glad/glad.h>
#include "VertexBuffer.h"


void VertexBuffer::Create(const void* data, unsigned int size) {
	glGenBuffers(1, &m_RendererId);
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}
void VertexBuffer::Destroy() const {
	glDeleteBuffers(1, &m_RendererId);
}
void VertexBuffer::Bind() const{
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);

}
void VertexBuffer::UnBind() const {
	glBindBuffer(GL_ARRAY_BUFFER, m_RendererId);
}