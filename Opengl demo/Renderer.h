#pragma once
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "shader_class.h"
#include "Model.h"
#include "Mesh.h"

class Renderer {
public:
	void Clear() const;
	void Clear(float* color) const;
	void Draw(const VertexArray& va, unsigned int size) const;
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
	void Draw(Model& model, Shader& shader);
	void Draw(Mesh& mesh, Shader& shader);
};