/*
 *	soundstream.cpp
 *	Sound Stream
 */

#include <chrono>
#include <Audio/AudioStream.hpp>
#include <Audio/Audio.hpp>
#include <Util/Logger.hpp>

namespace Audio
{
	AudioStream::AudioStream()
		: AudioSource(),
		  thread(nullptr),
		  streaming(false),
		  channelCount(0),
		  sampleRate(0),
		  format(0),
		  loop(false),
		  samplesProcessed(0)
	{

	}

	AudioStream::~AudioStream()
	{
		Stop();

		delete thread;
	}

	bool AudioStream::Initialize(unsigned int _channelCount, unsigned int _sampleRate)
	{
		format = System::GetFormatFromChannelCount(_channelCount);
		if (!format)
		{
			Error << "Unsupported channel count \"" << _channelCount << "\"." << lendl;
			return false;
		}

		channelCount = _channelCount;
		sampleRate = _sampleRate;

		return true;
	}

	void AudioStream::SoundStreamThread()
	{
		alGenBuffers(BUFFER_COUNT, buffers);
		for(size_t i = 0; i < BUFFER_COUNT; ++i)
			endBuffers[i] = false;

		bool requestStop = FillQueue();
		alSourcePlay(id);

		while(streaming)
		{
			if (GetStatus() == Stopped)
			{
				if (!requestStop)
					alSourcePlay(id);
				else
					streaming = false;
			}

			ALint buffersProcessed = 0;
			alGetSourcei(id, AL_BUFFERS_PROCESSED, &buffersProcessed);

			while(buffersProcessed--)
			{
				ALuint buffer;
				alSourceUnqueueBuffers(id, 1, &buffer);
				
				unsigned int bufferIndex = 0;
				for(size_t i = 0; i < BUFFER_COUNT; ++i)
				{
					if (buffers[i] == buffer)
					{
						bufferIndex = i;
						break;
					}
				}

				if (endBuffers[bufferIndex])
				{
					samplesProcessed = 0;
					endBuffers[bufferIndex] = false;
				}
				else
				{
					ALint size, bits;
					alGetBufferi(buffer, AL_SIZE, &size);
					alGetBufferi(buffer, AL_BITS, &bits);

					samplesProcessed += size / (bits / 8);
				}

				if (!requestStop)
				{
					if (FillAndPushBuffer(bufferIndex))
						requestStop = true;
				}
			}

			if (GetStatus() != Stopped)
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}

		alSourceStop(id);

		ClearQueue();
		alSourcei(id, AL_BUFFER, 0);
		alDeleteBuffers(BUFFER_COUNT, buffers);
	}

	void AudioStream::StartStreamThread()
	{
		if (thread)
		{
			delete thread;
			thread = nullptr;
		}

		streaming = true;
		thread = new std::thread(&AudioStream::SoundStreamThread, this);
	}

	bool AudioStream::FillAndPushBuffer(unsigned int index)
	{
		bool requestStop = false;
		Chunk chunk = { nullptr, 0 };

		if (!OnGetData(chunk))
		{
			endBuffers[index] = true;

			if (loop)
			{
				OnSeek(0);

				if (chunk.samples == nullptr || chunk.count == 0)
					return FillAndPushBuffer(index);
			}
			else
				requestStop = true;
		}

		if (chunk.samples != nullptr && chunk.count != 0)
		{
			unsigned int buffer = buffers[index];
			ALsizei size = static_cast<ALsizei>(chunk.count) * sizeof(short);

			alBufferData(buffer, format, chunk.samples, size, sampleRate);
			alSourceQueueBuffers(id, 1, &buffer);
		}

		return requestStop;
	}

	bool AudioStream::FillQueue()
	{
		for(size_t i = 0; i < BUFFER_COUNT; ++i)
		{
			if (FillAndPushBuffer(i))
				return true;
		}

		return false;
	}

	void AudioStream::ClearQueue()
	{
		ALint count;
		ALuint buffer;

		alGetSourcei(id, AL_BUFFERS_QUEUED, &count);
		while(count--)
			alSourceUnqueueBuffers(id, 1, &buffer);
	}

	void AudioStream::Play()
	{
		if (!format)
			return;

		if (streaming)
		{
			alSourcePlay(id);
			return;
		}

		OnSeek(0);

		samplesProcessed = 0;
		StartStreamThread();
	}

	void AudioStream::Pause()
	{
		alSourcePause(id);
	}

	void AudioStream::Stop()
	{
		streaming = false;

		if (thread && thread->joinable())
			thread->join();
	}

	void AudioStream::SetOffset(unsigned int value)
	{
		Stop();
		OnSeek(value);

		samplesProcessed = static_cast<unsigned long long>(value) * sampleRate * channelCount / 1000;
		StartStreamThread();
	}

	void AudioStream::SetLooping(bool value)
	{
		loop = value;
	}

	unsigned int AudioStream::GetOffset() const
	{
		ALfloat seconds;
		alGetSourcef(id, AL_SEC_OFFSET, &seconds);

		return static_cast<unsigned int>(1000 * seconds + 1000 * samplesProcessed / sampleRate / channelCount);
	}

	unsigned int AudioStream::GetChannelCount() const
	{
		return channelCount;
	}

	unsigned int AudioStream::GetSampleRate() const
	{
		return sampleRate;
	}

	bool AudioStream::GetLooping() const
	{
		return loop;
	}
}
