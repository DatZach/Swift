/*
 *	drawable.hpp
 *	Drawable
 */

#ifndef __VIDEO_DRAWABLE_HPP
#define __VIDEO_DRAWABLE_HPP

#include <Video/VertexBuffer.hpp>

namespace Video
{
	// TODO This is shit
	class IDrawable
	{
	public:
		virtual ~IDrawable() { }
		virtual void Draw(VertexBuffer& vertexBuffer) = 0;
	};
}

#endif
