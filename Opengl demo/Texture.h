#pragma once

class Texture {
private:
	static unsigned int m_nextID;

	unsigned int m_RendererID;
	unsigned int m_Index;
public:
	Texture(const char* fileName);
	~Texture();

	void Bind();
	void UnBind();

	inline int getIndex() const { return m_Index; };
};