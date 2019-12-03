/*
 *	vertex.hpp
 *	Vertex
 */

#ifndef __VIDEO_VERTEX_HPP
#define __VIDEO_VERTEX_HPP

#include <Util/Vector2.hpp>
#include <Video/Color.hpp>

namespace Video
{
	class Vertex
	{
	private:
#pragma pack(push, 1)
		typedef struct _OglVertex
		{
			float				x;
			float				y;
			float				r;
			float				g;
			float				b;
			float				a;
			float				tX;
			float				tY;
		} OglVertex;
#pragma pack(pop)

	public:
		static const unsigned int VertexSize = sizeof(OglVertex);

	public:
		Vector2f		position;
		Color			color;
		Vector2f		textureCoords;

		Vertex();
		Vertex(const Vector2f& position);
		Vertex(const Vector2f& position, const Color& color);
		Vertex(const Vector2f& position, const Vector2f& textureCoords);
		Vertex(const Vector2f& position, const Color& color, const Vector2f& textureCoords);

		void GetVertexData(unsigned char* buffer) const;
	};
}

#endif
