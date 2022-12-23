#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
class VertexArray {
private:
	unsigned int m_RendererID;
public:
	

	void AddVertexBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

	void Create() ;
	void Destroy() const ;
	void Bind() const;
	void UnBind() const;
};