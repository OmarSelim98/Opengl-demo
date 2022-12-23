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
	bool m_IsManualStrideEnabled = false;
	unsigned int m_Stride = 0;
	std::vector<VertexBufferElement> m_Elements;

public:
	VertexBufferLayout();

	template<typename T> void AddElement(unsigned int count);
	template<> void AddElement<float>(unsigned int count);
	template<> void AddElement<unsigned int>(unsigned int count);

	inline void SetStride(unsigned int stride) { m_IsManualStrideEnabled ? m_Stride = stride : m_Stride = m_Stride; };
	inline unsigned int GetStride() const { return m_Stride; };
	inline void SetStrideManually(bool value) { m_IsManualStrideEnabled = value; };
	inline std::vector<VertexBufferElement> GetElements() const& { return m_Elements; };
};