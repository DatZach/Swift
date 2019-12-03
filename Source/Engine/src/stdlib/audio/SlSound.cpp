/*
 *	stdlibsound.cpp
 *	Swift Standard Library
 *	Sound
 */

#include <StdLib/SlApi.hpp>
#include <Audio/Sound.hpp>
#include <Engine/Game.hpp>

namespace StdLib
{
	namespace Sound
	{
		/*
		 *	GetLooping()
		 *	Pause()
		 */

		void ConstructorFilename(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::String);

			std::string assetName = *stack.Pop();
			GameData::Sound* asset = Assets.sounds[assetName];
			if (asset == nullptr)
			{
				Error << "Sound asset \""
					  << assetName
					  << "\" doesn't exist."
					  << lendl;
				return;
			}

			if (asset->mode == GameData::Sound::SmMemory)
			{
				Warn << "Sound memory streaming not supported, use file streaming instead." << lendl;
				return;
			}

			Audio::Sound* sound = new Audio::Sound();
			if (sound->Open(asset->filename))
				parent->Field("handle") = Vm::Variant(sound, nullptr);
			else
			{
				Error << "Cannot play sound \""
					  << assetName
					  << "\", might not be able to open file \""
					  << asset->filename
					  << "\"."
					  << lendl;

				delete sound;
			}
		}

		void Play(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			Audio::Sound* sound = parent->Field("handle").EngineObject<Audio::Sound>();
			if (sound == nullptr)
			{
				Warn << "Sound cannot be played because no sound has been loaded." << lendl;
				return;
			}

			sound->SetLooping(false);
			sound->Play();
		}

		void Loop(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			Audio::Sound* sound = parent->Field("handle").EngineObject<Audio::Sound>();
			if (sound == nullptr)
			{
				Warn << "Sound cannot be played because no sound has been loaded." << lendl;
				return;
			}

			sound->SetLooping(true);
			sound->Play();
		}

		void Stop(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			Audio::Sound* sound = parent->Field("handle").EngineObject<Audio::Sound>();
			if (sound == nullptr)
			{
				Warn << "Sound cannot be stopped because no sound has been loaded." << lendl;
				return;
			}

			sound->Stop();
		}

		void SetVolume(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::Double);

			Audio::Sound* sound = parent->Field("handle").EngineObject<Audio::Sound>();
			if (sound == nullptr)
			{
				Warn << "Sound volume cannot be set because no sound has been loaded." << lendl;
				return;
			}

			sound->SetVolume(*stack.Pop());
		}

		void GetVolume(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			Audio::Sound* sound = parent->Field("handle").EngineObject<Audio::Sound>();
			if (sound == nullptr)
			{
				Warn << "Cannot return volume because no sound is loaded." << lendl;
				return;
			}

			StackPush(sound->GetVolume());
		}

		void SetPan(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::Double);

			Audio::Sound* sound = parent->Field("handle").EngineObject<Audio::Sound>();
			if (sound == nullptr)
			{
				Warn << "Sound pan cannot be set because no sound has been loaded." << lendl;
				return;
			}

			sound->SetPan(*stack.Pop());
		}

		void GetPan(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			Audio::Sound* sound = parent->Field("handle").EngineObject<Audio::Sound>();
			if (sound == nullptr)
			{
				Warn << "Cannot return pan because no sound is loaded." << lendl;
				return;
			}

			StackPush(sound->GetPan());
		}

		void SetPitch(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			ExpectType(0, Vm::Variant::Type::Double);

			Audio::Sound* sound = parent->Field("handle").EngineObject<Audio::Sound>();
			if (sound == nullptr)
			{
				Warn << "Sound pitch cannot be set because no sound has been loaded." << lendl;
				return;
			}

			sound->SetPitch(*stack.Pop());
		}

		void GetPitch(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			Audio::Sound* sound = parent->Field("handle").EngineObject<Audio::Sound>();
			if (sound == nullptr)
			{
				Warn << "Cannot return pitch because no sound is loaded." << lendl;
				return;
			}

			StackPush(sound->GetPitch());
		}

		void GetStatus(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			Audio::Sound* sound = parent->Field("handle").EngineObject<Audio::Sound>();
			if (sound == nullptr)
			{
				Warn << "Cannot return status because no sound is loaded." << lendl;
				return;
			}

			StackPush(sound->GetStatus());
		}

		void Deconstructor(Util::Stack<Vm::Variant*>& stack, Vm::Class* parent)
		{
			// TODO Don't deconstruct until sound is finished playing
			//delete parent->Field("handle").EngineObject<Audio::Sound>();
		}

		/*!
			class Sound

			properties
			Volume		get set double
			Pan			get set double
			Pitch		get set double
			Status		get int

			methods
			__Constructor	1
			Play			0
			Loop			0
			Stop			0
		 */
		void Install()
		{
			// Sound class
			Vm::Class soundClass;

			// Fields
			soundClass["handle"] = Vm::Variant(nullptr, nullptr);

			// Methods
			soundClass.AddMethod(VM_CLASS_CTOR_NAME, ConstructorFilename, 1);
			soundClass.AddMethod(VM_CLASS_DTOR_NAME, Deconstructor, 0);
			soundClass.AddMethod("Play", Play, 0);
			soundClass.AddMethod("Loop", Loop, 0);
			soundClass.AddMethod("Stop", Stop, 0);
			soundClass.AddMethod("__set_Volume", SetVolume, 1);
			soundClass.AddMethod("__get_Volume", GetVolume, 0);
			soundClass.AddMethod("__set_Pan", SetPan, 1);
			soundClass.AddMethod("__get_Pan", GetPan, 0);
			soundClass.AddMethod("__set_Pitch", SetPitch, 1);
			soundClass.AddMethod("__get_Pitch", GetPitch, 0);
			soundClass.AddMethod("__get_Status", GetStatus, 0);

			VirtualMachine.AddClass("Sound", soundClass);
		}
	}
}
