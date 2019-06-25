#pragma once

#include "VertexBuffer.h"

class VertexBufferLayout;

/// Contains VertexBuffer & VertexBufferLayout (metadata about buffer types,counts,normalized)
class VertexArray {
private:
	unsigned int m_RendererID;
public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

	void Bind() const;
	void Unbind() const;
};