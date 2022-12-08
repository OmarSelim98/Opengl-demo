#include "VertexBufferLayout.h"
VertexBufferLayout::VertexBufferLayout() {}

template<typename T>
void VertexBufferLayout::AddElement(unsigned int count) {
	std::runtime_error(false, "ok");
}
template<> void VertexBufferLayout::AddElement<float>(unsigned int count) {
	m_Elements.push_back({ count, GL_FLOAT, GL_FALSE });
	if (!m_IsManualStrideEnabled) {
		m_Stride += count * HELPER::sizeOfGL(GL_FLOAT);
	}
}
template<> void VertexBufferLayout::AddElement<unsigned int>(unsigned int count) {
	m_Elements.push_back({ count, GL_UNSIGNED_INT, GL_FALSE });
	if (!m_IsManualStrideEnabled) {
		m_Stride += count * HELPER::sizeOfGL(GL_UNSIGNED_INT);
	}
}