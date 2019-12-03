/*
 *	gamedata.hpp
 *	Game Data Deserializer
 */

#ifndef __GAME_DATA_HPP
#define __GAME_DATA_HPP

#include <string>
#include <GameData/configuration.hpp>
#include <GameData/drm.hpp>
#include <GameData/textures.hpp>
#include <GameData/sounds.hpp>
#include <GameData/bytecodeobjects.hpp>
#include <GameData/scenes.hpp>
#include <GameData/texturesheets.hpp>
#include <GameData/audio.hpp>

#define GD_FILE_MASK					"*.sgd"

#if defined WINDOWS
	#define ENGINE_TARGETED_PLATFORM		PfWindows
#elif defined MACOSX
	#define ENGINE_TARGETED_PLATFORM		PfMacOSX
#elif defined LINUX
	#define ENGINE_TARGETED_PLATFORM		PfLinux
#elif defined ANDROID
	#define ENGINE_TARGETED_PLATFORM		PfAndroid
#elif defined IOS
	#define ENGINE_TARGETED_PLATFORM		PfIOS
#else
	#define ENGINE_TARGETED_PLATFORM		PfUnknown
#endif

namespace GameData
{
	class GameData
	{
	public:
		enum PlatformFlags
		{
			PfWindows	= 0x01,
			PfMacOSX	= 0x02,
			PfLinux		= 0x04,
			PfAndroid	= 0x08,
			PfIOS		= 0x10,
			PfHtml5		= 0x20,
			PfUndefined	= 0x40,
			PfUnknown	= 0x80
		};

	public:
		unsigned char	platform;
		Configuration	config;
		Drm				drm;
		Textures		textures;
		Sounds			sounds;
		BytecodeObjects	bytecodeObjects;
		Scenes			scenes;
		TextureSheets	textureSheets;
		Audio			audio;

	public:
		GameData();

		bool Load(const std::string& filename);
	};
}

#endif
