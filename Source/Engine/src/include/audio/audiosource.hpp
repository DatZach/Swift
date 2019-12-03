/*
 *	soundsource.hpp
 *	Sound Source
 */

#ifndef __AUDIO_AUDIO_SOURCE_HPP
#define __AUDIO_AUDIO_SOURCE_HPP

namespace Audio
{
	class AudioSource
	{
	public:
		enum Status
		{
			Stopped,
			Paused,
			Playing
		};

	protected:
		unsigned int id;

		AudioSource();

	public:
		virtual ~AudioSource();

		Status GetStatus() const;

		void SetPitch(float value) const;
		void SetVolume(float value) const;
		void SetPan(float value) const;

		float GetPitch() const;
		float GetVolume() const;
		float GetPan() const;
	};
}

#endif
