/*
 *	sprite.hpp
 *	Sprite
 */

#ifndef __SPRITE_HPP
#define __SPRITE_HPP

#include <Video/VertexBuffer.hpp>
#include <Video/Texture.hpp>
#include <Video/Drawable.hpp>
#include <GameData/Textures.hpp>
#include <Util/Rectangle.hpp>

namespace Video
{
	class Sprite : public IDrawable
	{
	private:
		size_t				currentFrameIndex;
		double				currentFrameTime;

		void UploadTexture(size_t frameIndex, VertexBuffer& vertexBuffer);

	public:
		GameData::Texture	gdTexture;
		Vector2f			position;
		bool				animated;
		double				frameSpeed;
		Color				color;
		Vector2f			scale;
		float				rotation;
		bool				smooth;

	public:
		Sprite(const GameData::Texture& gdTexture);

		virtual void Draw(VertexBuffer& vertexBuffer);

		FloatRect GetBoundingBox() const;
	};
}

#endif
