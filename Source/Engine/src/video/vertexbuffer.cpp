/*
 *	vertexbuffer.cpp
 *	Vertex Buffer
 */

#include <Video/VertexBuffer.hpp>

namespace Video
{
	VertexBuffer::VertexBuffer(size_t verticeCount)
		: vertices(),
		  buffer(nullptr),
		  size(0),
		  dirty(false)
	{
		// Probably only ever going to see 4 at most
		vertices.reserve(verticeCount);
	}

	VertexBuffer::~VertexBuffer()
	{
		delete[] buffer;
	}

	void VertexBuffer::UpdateBuffer()
	{
		size_t length = vertices.size();
		size = length * Vertex::VertexSize;
		
		delete[] buffer;
		buffer = new unsigned char[size];

		unsigned char* walkBuffer = buffer;
		for(size_t i = 0; i < length; ++i, walkBuffer += Vertex::VertexSize)
			vertices[i].GetVertexData(walkBuffer);

		dirty = false;
	}

	void VertexBuffer::SetDirty()
	{
		dirty = true;
	}

	size_t VertexBuffer::Add(const Vertex& vertex)
	{
		vertices.push_back(vertex);
		dirty = true;
	
		return vertices.size() - 1;
	}

	void VertexBuffer::Remove(size_t index)
	{
		if (index >= vertices.size())
			return;

		vertices.erase(vertices.cbegin() + index);
		dirty = true;
	}

	void VertexBuffer::Clear(size_t size)
	{
		vertices.clear();
		vertices.resize(size);
		dirty = true;
	}

	const void* VertexBuffer::GetBuffer()
	{
		if (dirty)
			UpdateBuffer();

		return buffer;
	}

	size_t VertexBuffer::GetSize() const
	{
		/*
		 *	Note that we don't update the buffer when getting the size
		 *      if it's dirty but we get the size old data (still valid)
		 *		data might be overrun resulting in a crash or visual bug.
		 */
		
		return size;
	}

	size_t VertexBuffer::Count() const
	{
		return vertices.size();
	}

	Vertex& VertexBuffer::operator[](size_t i)
	{
		return vertices[i];
	}
}
