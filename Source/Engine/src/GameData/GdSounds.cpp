/*
 *	gdsounds.cpp
 *	Sounds
 */

#include <GameData/Sounds.hpp>

namespace GameData
{
	Sound::Sound()
		: IAsset(),
		  name(""),
		  mode(SmStream),
		  audioId(0),
		  filename("")
	{

	}

	void Sound::Read(Util::Stream* stream)
	{
		name = stream->ReadString();
		mode = stream->ReadByte();

		switch(mode)
		{
			case SmStream:
				filename = stream->ReadString();
				break;

			case SmMemory:
				audioId = stream->ReadDword();
				break;
		}
	}

	Sounds::Sounds()
		: IAsset(),
		  sounds()
	{

	}

	Sounds::~Sounds()
	{
		for(Sound* sound : sounds)
			delete sound;
	}

	void Sounds::Read(Util::Stream* stream)
	{
		unsigned int count = stream->ReadDword();

		while(count--)
		{
			Sound* sound = new Sound();
			sound->Read(stream);

			sounds.push_back(sound);
		}
	}

	size_t Sounds::size() const
	{
		return sounds.size();
	}

	Sound* Sounds::operator[](const std::string& value) const
	{
		for(Sound* sound : sounds)
		{
			if (sound->name == value)
				return sound;
		}

		return nullptr;
	}

	Sound* Sounds::operator[](size_t i) const
	{
		return sounds[i];
	}
}
