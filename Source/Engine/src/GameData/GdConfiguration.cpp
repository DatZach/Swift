/*
 *	gdconfiguration.cpp
 *	Game Data Configuration
 */

#include <GameData/Configuration.hpp>

namespace GameData
{
	Configuration::Configuration()
		: IAsset(),
		  GameTitle("Untitled Game"),
		  Size(800, 600),
		  Fullscreen(false),
		  Vsync(true),
		  Cursor(true),
		  AntiAliasingLevel(0),
		  TargetFramerate(0),
		  InitialState("")
	{

	}

	void Configuration::Read(Util::Stream* stream)
	{
		GameTitle			= stream->ReadString();
		Size.x				= stream->ReadWord();
		Size.y				= stream->ReadWord();
		Fullscreen			= stream->ReadBoolean();
		Vsync				= stream->ReadBoolean();
		Cursor				= stream->ReadBoolean();
		AntiAliasingLevel	= stream->ReadByte();
		TargetFramerate		= stream->ReadByte();
		InitialState		= stream->ReadString();
	}
}
