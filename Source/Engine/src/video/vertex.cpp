/*
 *	vertex.cpp
 *	Vertex
 */

#include <Video/Vertex.hpp>

namespace Video
{
	Vertex::Vertex()
		: position(),
		  color(Color::White),
		  textureCoords(-1, -1)
	{

	}

	Vertex::Vertex(const Vector2f& position)
		: position(position),
		  color(Color::White),
		  textureCoords(-1, -1)
	{

	}

	Vertex::Vertex(const Vector2f& position, const Color& color)
		: position(position),
		  color(color),
		  textureCoords(-1, -1)
	{

	}

	Vertex::Vertex(const Vector2f& position, const Vector2f& textureCoords)
		: position(position),
		  color(Color::White),
		  textureCoords(textureCoords)
	{

	}

	Vertex::Vertex(const Vector2f& position, const Color& color, const Vector2f& textureCoords)
		: position(position),
		  color(color),
		  textureCoords(textureCoords)
	{

	}

	void Vertex::GetVertexData(unsigned char* buffer) const
	{
		OglVertex* oglVertex = reinterpret_cast<OglVertex*>(buffer);

		oglVertex->x = position.x;
		oglVertex->y = position.y;
		oglVertex->r = color.r;
		oglVertex->g = color.g;
		oglVertex->b = color.b;
		oglVertex->a = color.a;
		oglVertex->tX = textureCoords.x;
		oglVertex->tY = textureCoords.y;
	}
}
