/*
 *	gamedata.cpp
 *	Game Data Deserializer
 */

#include <GameData/GameData.hpp>
#include <Util/Logger.hpp>
#include <Util/StopWatch.hpp>

#define GD_MAGIC			GD_TAG_VALUE('S', 'W', 'A', 'G')
#define GD_VERSION			11

namespace GameData
{
	GameData::GameData()
		: platform(PfUnknown),
		  config(),
		  drm(),
		  textures(),
		  sounds(),
		  bytecodeObjects(),
		  textureSheets(),
		  audio()
	{

	}

	bool GameData::Load(const std::string& filename)
	{
		Util::Stopwatch loadTime;
		loadTime.Start();

		Trace << "Loading game data..." << lendl;

		// Note that we do not catch exceptions here because we catch them in main()
		Util::Stream* stream = new Util::Stream(filename, Util::Stream::ModeRead);

		// Verify header
		if (stream->ReadDword() != GD_MAGIC)
		{
			Error << "This is not a valid GameData file." << lendl;
			delete stream;
			return false;
		}

		unsigned short version = stream->ReadWord();
		if (version != GD_VERSION)
		{
			Error << "GameData version " << version << " unsupported." << lendl;
			delete stream;
			return false;
		}

		platform = stream->ReadByte();
		if ((platform & ~ENGINE_TARGETED_PLATFORM))
			Warn << "GameData was built for another platform, may not run or run with issues." << lendl;

		// Read all sections
		unsigned char sectionCount = stream->ReadByte();
		while(sectionCount-- && !stream->EndOfStream())
		{
			unsigned int tag = stream->ReadDword();

			switch(tag)
			{
				case GD_TAG_CONFIG:
					config.Read(stream);
					break;

				case GD_TAG_DRM:
					drm.Read(stream);
					break;

				case GD_TAG_TEXTURES:
					textures.Read(stream);
					break;

				case GD_TAG_SOUNDS:
					sounds.Read(stream);
					break;

				case GD_TAG_CODE:
					bytecodeObjects.Read(stream);
					break;

				case GD_TAG_SCENES:
					scenes.Read(stream);
					break;

				case GD_TAG_TEXTURE_SHEETS:
					textureSheets.Read(stream);
					break;

				case GD_TAG_AUDIO:
					audio.Read(stream);
					break;

				default:
					Warn << "GameData Unknown tag \""
						 << (char)(tag & 0xFF)
						 << (char)((tag >> 8) & 0xFF)
						 << (char)((tag >> 16) & 0xFF)
						 << (char)((tag >> 24) & 0xFF)
						 << "\"."
						 << lendl; 
			}
		}

		delete stream;

		loadTime.Stop();
		VerboseTrace << "Loaded game data in "
					 << loadTime.GetElapsedMilliseconds()
					 << "ms."
					 << lendl;

		return true;
	}
}
