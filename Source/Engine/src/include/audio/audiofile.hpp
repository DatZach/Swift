/*
 *	soundfile.hpp
 *	Sound File
 */

#ifndef __AUDIO_SOUND_FILE_HPP
#define __AUDIO_SOUND_FILE_HPP

#include <string>
#include <Audio/Codec.hpp>

namespace Audio
{
	class AudioFile
	{
	private:
		std::vector<Codec*> codecs;
		Codec* currentCodec;

	public:
		AudioFile();
		~AudioFile();

		bool Open(const std::string& filename);
		size_t Read(short* data, size_t count);
		void Seek(unsigned int offset);

		size_t GetSampleCount() const;
		size_t GetChannelCount() const;
		size_t GetSampleRate() const;
	};
}

#endif
