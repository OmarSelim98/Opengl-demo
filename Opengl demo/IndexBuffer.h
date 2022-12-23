#pragma once

class IndexBuffer {
private:
	unsigned int m_RendererId;
	unsigned int m_Count;
public:

	void Create(const void* data, unsigned int count);
	void Destroy() const;
	void Bind();
	void UnBind();

	inline unsigned int GetCount() const { return m_Count; }
};