/*
 *	sound.hpp
 *	Sound
 */

#ifndef __AUDIO_SOUND_HPP
#define __AUDIO_SOUND_HPP

#include <vector>
#include <string>
#include <mutex>
#include <Audio/AudioStream.hpp>
#include <Audio/AudioFile.hpp>

namespace Audio
{
	class Sound : public AudioStream
	{
	private:
		std::mutex mutex;
		AudioFile file;
		std::vector<short> samples;

	protected:
		 virtual bool OnGetData(Chunk& chunk);
		 virtual void OnSeek(unsigned int offset);

	public:
		Sound();
		~Sound();

		bool Open(const std::string& filename);
	};
}

#endif
