#include <glad/glad.h>
#include "VertexArray.h"


void VertexArray::AddVertexBuffer(const VertexBuffer& vb,const VertexBufferLayout& layout)
{
	vb.Bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++) {
		glVertexAttribPointer(i, elements[i].count, elements[i].type, elements[i].normalized, layout.GetStride(), (void*)(offset));
		glEnableVertexAttribArray(i);
		offset += elements[i].count * HELPER::sizeOfGL(elements[i].type);
	}
}
void VertexArray::Create()
{
	glGenVertexArrays(1, &m_RendererID);
}

void VertexArray::Destroy() const
{
	glDeleteVertexArrays(1, &m_RendererID);
}

void VertexArray::Bind() const
{
	glBindVertexArray(m_RendererID);
}

void VertexArray::UnBind() const
{
	glBindVertexArray(0);
}
