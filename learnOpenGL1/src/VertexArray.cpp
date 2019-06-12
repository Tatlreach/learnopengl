#include "VertexArray.h"

#include "Renderer.h"

VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_RendererID));
	GLCall(glBindVertexArray(m_RendererID));
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout & layout)
{
	Bind();
	vb.Bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++) {

		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i));
		//links buffer with VAO
		GLCall(glVertexAttribPointer(i,	//index, starting index of specified attribute
			element.count,				//size, number of components in ea. vertex
			element.type,				//type, Attribute Type
			element.normalized,			//normalized
			layout.GetStride(),			//stride, total size of vertex
			(const void*)offset));		//offset for the attribute WITHIN the vertex e.g. (const void*)
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}

}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::UnBind() const
{
	GLCall(glBindVertexArray(0));
}
