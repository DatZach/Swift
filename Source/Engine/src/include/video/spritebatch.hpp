/*
 *	spritebatch.hpp
 *	Sprite Batch
 */

#ifndef __VIDEO_SPRITE_BATCH_HPP
#define __VIDEO_SPRITE_BATCH_HPP

#include <vector>
#include <Video/Sprite.hpp>

#define SpriteBatch		Video::SpriteBatchSingleton::GetInstance()

namespace Video
{
	class SpriteBatchSingleton
	{
	private:
		VertexBuffer vertexBuffer;
		std::vector<Sprite*> sprites;

	public:
		SpriteBatchSingleton();

		void Begin();
		void End();
		void Draw(Sprite* sprite);
		void Render();

		static SpriteBatchSingleton& GetInstance();
	};
}

#endif
