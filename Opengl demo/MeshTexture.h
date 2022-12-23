#pragma once
#include <iostream>

enum TEXTURE_TYPES {DIFFUSE,SPECULAR};

class MeshTexture {
private:
	unsigned int m_RendererID;
	unsigned int m_Index;
	TEXTURE_TYPES m_Type;
public:
	MeshTexture(std::string fileName, unsigned int index, TEXTURE_TYPES type);

	void Destroy() const;
	void Bind();
	void UnBind();

	inline int getIndex() const { return m_Index; };
	inline TEXTURE_TYPES getType() const { return m_Type; };
};