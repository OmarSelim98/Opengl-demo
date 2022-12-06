#pragma once

#include <iostream>
#include<glad/glad.h>
#include <vector>
#include "Helper.h"

struct VertexBufferElement {
	unsigned int count;
	unsigned int type;
	unsigned int normalized;
};

class VertexBufferLayout {
private:
	unsigned int m_Stride;
	std::vector<VertexBufferElement> m_Elements;

public:
	VertexBufferLayout();

	template<typename T> void AddElement(unsigned int count);
	template<> void AddElement<float>(unsigned int count);
	template<> void AddElement<unsigned int>(unsigned int count);

	inline unsigned int GetStride() const { return m_Stride; };
	inline std::vector<VertexBufferElement> GetElements() const& { return m_Elements; };
};