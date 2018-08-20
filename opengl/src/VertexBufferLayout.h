#pragma once
#include <vector>
#include <GL/glew.h>
#include "GLDebug.h"
struct VertextBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT:	return sizeof(GLfloat);
		case GL_UNSIGNED_INT:	return sizeof(GLuint);
		case GL_UNSIGNED_BYTE:	return sizeof(GLbyte);
		}
			ASSERT(false);
		return 0;
	}
};
class VertexBufferLayout
{
private:
	std::vector<VertextBufferElement> m_Elements;
	unsigned int m_Stride;
public:
	VertexBufferLayout() :m_Stride(0) {}
	~VertexBufferLayout() {}

	template<typename T>
	void Push(unsigned int count)
	{
		static_assert(false);
	}
	template<>
	void Push<float>(unsigned int count)
	{
		m_Elements.push_back({ GL_FLOAT,count,GL_FALSE });
		m_Stride += VertextBufferElement::GetSizeOfType(GL_FLOAT) * count;
	}
	template<>
	void Push<unsigned int>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_INT,count,GL_FALSE });
		m_Stride += VertextBufferElement::GetSizeOfType(GL_UNSIGNED_INT) * count;
	}
	template<>
	void Push<unsigned char>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_BYTE,count,GL_TRUE });
		m_Stride += VertextBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE) * count;
	}
	inline const std::vector<VertextBufferElement>& GetElements() const { return m_Elements; }
	inline unsigned int GetStride() const { return m_Stride; }
};

