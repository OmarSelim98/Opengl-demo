#pragma once

class VertexBuffer {
private:
	unsigned int m_RendererId;
public:

	void Create(const void* data, unsigned int size);
	void Destroy() const ;
	void Bind() const;
	void UnBind() const;
};