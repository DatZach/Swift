/*
 *	sounds.hpp
 *	Sound Assets
 */

#ifndef __GAME_DATA_SOUNDS_HPP
#define __GAME_DATA_SOUNDS_HPP

#include <string>
#include <vector>
#include <GameData/Asset.hpp>

#define GD_TAG_SOUNDS			GD_TAG_VALUE('S', 'O', 'N', 'D')

namespace GameData
{
	class Sound : public IAsset
	{
	public:
		enum SoundMode
		{
			SmStream,
			SmMemory
		};

	public:
		std::string		name;
		unsigned char	mode;
		size_t			audioId;
		std::string		filename;

	public:
		Sound();

		virtual void Read(Util::Stream* stream);
	};

	class Sounds : public IAsset
	{
	private:
		std::vector<Sound*> sounds;

	public:
		Sounds();
		~Sounds();

		virtual void Read(Util::Stream* stream);
		
		size_t size() const;
		Sound* operator[](const std::string& value) const;
		Sound* operator[](size_t i) const;
	};
}

#endif
