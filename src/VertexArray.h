#pragma once
#include "VertexBufferLayout.h"
#include "VertexBuffer.h"
class VertexArray
{
private:
	unsigned int m_Renderer;
public:
	VertexArray();
	~VertexArray();
	void Bind() const;
	void Unbind() const;
	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
};

