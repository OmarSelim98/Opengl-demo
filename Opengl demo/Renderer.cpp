#include "Renderer.h"

void Renderer::Clear() const {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::Clear(float* color) const {
	glClearColor(color[0], color[1], color[2], 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}
void Renderer::Draw(const VertexArray& va, unsigned int size) const {
	va.Bind();
	glDrawArrays(GL_TRIANGLES, 0, size);
	va.UnBind();
}
void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const {
	va.Bind();
	//glDrawArrays(GL_TRIANGLES, 0, 3);
	glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, 0);
	va.UnBind();
}