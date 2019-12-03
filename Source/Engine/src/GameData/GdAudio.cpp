/*
 *	gdaudio.cpp
 *	Game Data Audio
 */

#include <GameData/Audio.hpp>

namespace GameData
{
	Audio::Audio()
		: IAsset(),
		  audioStreams()
	{

	}

	Audio::~Audio()
	{
		for(Util::Stream* audioStream : audioStreams)
			delete audioStream;
	}

	void Audio::Read(Util::Stream* stream)
	{
		size_t count = stream->ReadDword();
		while(count--)
			audioStreams.push_back(stream->Deserialize(false));
	}

	size_t Audio::size() const
	{
		return audioStreams.size();
	}

	Util::Stream* Audio::operator[](size_t i) const
	{
		return audioStreams[i];
	}
}
