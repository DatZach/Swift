/*
 *	spritebatch.cpp
 *	Sprite Batch
 */

#include <Video/SpriteBatch.hpp>

#define SPRITE_BATCH_RESERVE_COUNT		16

namespace Video
{
	SpriteBatchSingleton::SpriteBatchSingleton()
		: vertexBuffer(),
		  sprites()
	{

	}

	void SpriteBatchSingleton::Begin()
	{
		vertexBuffer.Clear(SPRITE_BATCH_RESERVE_COUNT * 4);
	}

	void SpriteBatchSingleton::End()
	{
		// Fuck you
		//for(Sprite* sprite : sprites)
		//	delete sprite;

		sprites.clear();
		sprites.resize(SPRITE_BATCH_RESERVE_COUNT);
	}

	void SpriteBatchSingleton::Draw(Sprite* sprite)
	{
		// HACK Add sprites to manage
		bool canAdd = true;
		for(Sprite* mSprite : sprites)
		{
			if (mSprite == sprite)
			{
				canAdd = false;
				break;
			}
		}

		if (canAdd)
			sprites.push_back(sprite);

		// Draw sprite to vertex buffer
		sprite->Draw(vertexBuffer);
	}

	void SpriteBatchSingleton::Render()
	{
		size_t count = vertexBuffer.GetSize() / 4;
		std::vector<int> indices(count * 6);

		for(size_t i = 0, j = 0; i < count; ++i, j += 4)
		{
			indices.push_back(j);
			indices.push_back(j + 1);
			indices.push_back(j + 2);
			indices.push_back(j + 2);
			indices.push_back(j + 3);
			indices.push_back(j);
		}

		// Draw batch
		glBufferData(GL_ARRAY_BUFFER, vertexBuffer.GetSize(), vertexBuffer.GetBuffer(), GL_DYNAMIC_DRAW);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, indices.data());
	}

	SpriteBatchSingleton& SpriteBatchSingleton::GetInstance()
	{
		static SpriteBatchSingleton instance;
		return instance;
	}
}
