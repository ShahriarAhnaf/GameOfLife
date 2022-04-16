#include "VertexArray.h"
#include "Renderer.h"
VertexArray::VertexArray()
{
	GLcall(glGenVertexArrays(1, &m_Renderer));
}

VertexArray::~VertexArray()
{
	GLcall(glDeleteVertexArrays(1, &m_Renderer));
}

void VertexArray::Bind() const{
	GLcall(glBindVertexArray(m_Renderer));
}

void VertexArray::Unbind() const
{
	GLcall(glBindVertexArray(0));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();

	// iterate through each element(struct) and put it into the array buffer
	const auto& elements = layout.getElements();
	unsigned int offset = 0; // to increment after each
	for (unsigned int i = 0; i < elements.size(); i++) {
		//got thru the vector of elements
		const auto& element = elements[i];
		// enable the appropriate array for index
		GLcall(glEnableVertexAttribArray(i));
		GLcall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*) offset));
		//enable the vertex
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type); // adding the size 
	}
}
