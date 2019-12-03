/*
 *	vertexbuffer.hpp
 *	Vertex Buffer
 */

#ifndef __VIDEO_VERTEX_BUFFER_HPP
#define __VIDEO_VERTEX_BUFFER_HPP

#include <vector>
#include <Video/Vertex.hpp>

namespace Video
{
	class VertexBuffer
	{
	private:
		std::vector<Vertex> vertices;
		unsigned char* buffer;
		size_t size;
		bool dirty;

		void UpdateBuffer();

	public:
		VertexBuffer(size_t verticeCount = 4);
		~VertexBuffer();

		void SetDirty();

		size_t Add(const Vertex& vertex);
		void Remove(size_t index);
		void Clear(size_t size = 0);

		const void* GetBuffer();
		size_t GetSize() const;

		size_t Count() const;

		Vertex& operator[](size_t i);
	};
}

#endif