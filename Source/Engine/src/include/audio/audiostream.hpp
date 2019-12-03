/*
 *	soundstream.hpp
 *	Sound Stream
 */

#ifndef __AUDIO_AUDIO_STREAM_HPP
#define __AUDIO_AUDIO_STREAM_HPP

#include <Audio/AudioSource.hpp>
#include <thread>

namespace Audio
{
	class AudioStream : public AudioSource
	{
	private:
		static const unsigned int BUFFER_COUNT = 3;
	
	public:
		typedef struct _Chunk
		{
			const short*	samples;
			size_t			count;
		} Chunk;

	private:
		std::thread*		thread;
		bool				streaming;
		unsigned int		buffers[BUFFER_COUNT];
		bool				endBuffers[BUFFER_COUNT];
		unsigned int		channelCount;
		unsigned int		sampleRate;
		unsigned int		format;
		bool				loop;
		unsigned long long	samplesProcessed;

		void SoundStreamThread();
		void StartStreamThread();
		bool FillAndPushBuffer(unsigned int index);
		bool FillQueue();
		void ClearQueue();

	protected:
		AudioStream();
		~AudioStream();

		bool Initialize(unsigned int _channelCount, unsigned int _sampleRate);

		virtual bool OnGetData(Chunk& data) = 0;
		virtual void OnSeek(unsigned int offset) = 0;

	public:
		void Play();
		void Pause();
		void Stop();

		void SetOffset(unsigned int value);
		void SetLooping(bool value);

		unsigned int GetChannelCount() const;
		unsigned int GetSampleRate() const;
		unsigned int GetOffset() const;
		bool GetLooping() const;
	};
}

#endif
