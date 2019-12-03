/*
 *	sprite.cpp
 *	Sprite
 */

#include <Video/Sprite.hpp>
#include <Video/SheetCache.hpp>
#include <Video/SpriteBatch.hpp>
#include <Video/Video.hpp>

// TODO Use rectangles in sprites

#define FRAME_TIME_CEILING			1.0

namespace Video
{
	Sprite::Sprite(const GameData::Texture& gdTexture)
		: gdTexture(gdTexture),
		  currentFrameIndex(0),
		  currentFrameTime(0.0),
		  position(0.0f, 0.0f),
		  animated(false),
		  frameSpeed(0.0),
		  color(Color::White),
		  scale(1.0f, 1.0f),
		  rotation(0.0f),
		  smooth(false)
	{
		
	}

	void Sprite::UploadTexture(size_t frameIndex, VertexBuffer& vertexBuffer)
	{
		GameData::Frame& frame = gdTexture.frames[frameIndex % gdTexture.frames.size()];
		Texture* textureSheet = SheetCache[frame.sheetIndex];

		textureSheet->SetLinear(smooth);
		Vector2i sheetSize = textureSheet->GetSize();

		// TODO Rotation

		float angle = -rotation * 3.141592654f / 180.0f;
		float rotCos = static_cast<float>(cos(angle));
		float rotSin = static_cast<float>(cos(angle));
		float x = frame.positionX;
		float y = frame.positionY;
		float width = gdTexture.width;
		float height = gdTexture.height;

		float tX1 = x / sheetSize.x;
		float tY1 = y / sheetSize.y;
		float tX2 = (x + width) / sheetSize.x;
		float tY2 = (y + height) / sheetSize.y;
		
		// Top Left
		vertexBuffer.Add(
			Vertex(
				Vector2f(
					position.x - gdTexture.originX * scale.x,
					position.y - gdTexture.originY * scale.y
				),
				color,
				Vector2f(tX1, tY1)
			)
		);
		
		// Top Right
		vertexBuffer.Add(
			Vertex(
				Vector2f(
					position.x - gdTexture.originX * scale.x + width * scale.x,
					position.y - gdTexture.originY * scale.y
				),
				color,
				Vector2f(tX2, tY1)
			)
		);

		// Bottom Right
		vertexBuffer.Add(
			Vertex(
				Vector2f(
					position.x - gdTexture.originX * scale.x + width * scale.x,
					position.y - gdTexture.originY * scale.y + height * scale.y
				),
				color,
				Vector2f(tX2, tY2)
			)
		);

		// Bottom Left
		vertexBuffer.Add(
			Vertex(
				Vector2f(
					position.x - gdTexture.originX * scale.x,
					position.y - gdTexture.originY * scale.y + height * scale.y
				),
				color,
				Vector2f(tX1, tY2)
			)
		);

		textureSheet->Bind();
	}

	void Sprite::Draw(VertexBuffer& vertexBuffer)
	{
		UploadTexture(currentFrameIndex, vertexBuffer);

		if (animated)
		{
			currentFrameTime += frameSpeed;
			if (currentFrameTime >= FRAME_TIME_CEILING)
			{
				++currentFrameIndex;
				currentFrameTime = 0.0;
			}
			else if (currentFrameTime <= -FRAME_TIME_CEILING)
			{
				--currentFrameIndex;
				currentFrameTime = 0.0;
			}
		}
	}

	FloatRect Sprite::GetBoundingBox() const
	{
		// TODO Override with GD's bbox data
		return FloatRect(position.x - gdTexture.originX, position.y - gdTexture.originY, gdTexture.width, gdTexture.height);
	}
}
