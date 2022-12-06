#include <glad/glad.h>
#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(const void* data, unsigned int count) :m_Count(count) {
	glGenBuffers(1, &m_RendererId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
}
IndexBuffer::~IndexBuffer() {
	glDeleteBuffers(1, &m_RendererId);
}
void IndexBuffer::Bind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererId);

}
void IndexBuffer::UnBind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererId);
}