/*
 *	audio.hpp
 *	Game Data Audio
 */

#ifndef __GAME_DATA_AUDIO_HPP
#define __GAME_DATA_AUDIO_HPP

#include <vector>
#include <GameData/Asset.hpp>

#define GD_TAG_AUDIO			GD_TAG_VALUE('A', 'U', 'D', 'O')

namespace GameData
{
	class Audio : public IAsset
	{
	private:
		std::vector<Util::Stream*> audioStreams;

	public:
		Audio();
		~Audio();

		virtual void Read(Util::Stream* stream);

		size_t size() const;
		Util::Stream* operator[](size_t i) const;
	};
}

#endif
