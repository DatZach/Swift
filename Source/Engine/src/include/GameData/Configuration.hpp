/*
 *	configuration.hpp
 *	Game Engine Configuration
 */

#ifndef __GAME_DATA_CONFIGURATION_HPP
#define __GAME_DATA_CONFIGURATION_HPP

#include <string>
#include <GameData/Asset.hpp>
#include <Util/Vector2.hpp>

#define GD_TAG_CONFIG			GD_TAG_VALUE('C', 'N', 'F', 'G')

namespace GameData
{
	class Configuration : public IAsset
	{
	public:
		enum PlatformFlags
		{
			PfNone		= 0x00,
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
		std::string			GameTitle;
		Vector2u			Size;
		bool				Fullscreen;
		bool				Vsync;
		bool				Cursor;
		int					AntiAliasingLevel;
		int					TargetFramerate;
		std::string			InitialState;

	public:
		Configuration();

		virtual void Read(Util::Stream* stream);
	};
}

#endif
