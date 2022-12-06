#pragma once
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "shader_class.h"

class Renderer {
public:
	void Clear() const;
	void Clear(float* color) const;
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};