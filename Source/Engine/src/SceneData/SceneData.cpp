/*
 *	scenedata.cpp
 *	Scene Data Deserializer
 */

#include <SceneData/SceneData.hpp>
#include <Util/Stream.hpp>
#include <Util/StopWatch.hpp>
#include <Util/Logger.hpp>

#define SD_MAGIC				SD_TAG_VALUE('S', 'W', 'F', 'L')
#define SD_VERSION				3

namespace SceneData
{
	SceneData::SceneData()
		: configuration(),
		  background(),
		  tiles(),
		  entities()
	{

	}

	bool SceneData::Load(Util::Stream* stream)
	{
		// Read header
		if (stream->ReadDword() != SD_MAGIC)
		{
			Error << "This is not a valid SceneData file." << lendl;
			return false;
		}

		unsigned short version = stream->ReadWord();
		if (version != SD_VERSION)
		{
			Error << "SceneData version " << version << " unsupported." << lendl;
			return false;
		}

		// Read all sections
		unsigned char sectionCount = stream->ReadByte();
		while(sectionCount-- && !stream->EndOfStream())
		{
			unsigned int tag = stream->ReadDword();

			switch(tag)
			{
				case SD_TAG_CONFIG:
					configuration.Read(stream);
					break;

				case SD_TAG_BACKGROUND:
					background.Read(stream);
					break;

				case SD_TAG_TILES:
					tiles.Read(stream);
					break;

				case SD_TAG_ENTITIES:
					entities.Read(stream);
					break;

				default:
					Warn << "SceneData Unknown tag \""
						 << (char)(tag & 0xFF)
						 << (char)((tag >> 8) & 0xFF)
						 << (char)((tag >> 16) & 0xFF)
						 << (char)((tag >> 24) & 0xFF)
						 << "\"."
						 << lendl; 
			}
		}

		return true;
	}

	bool SceneData::Load(const std::string& filename)
	{
		Util::Stream* stream;

		// Open file for streaming
		try
		{
			stream = new Util::Stream(filename, Util::Stream::ModeRead);
			if (stream == NULL)
				return false;
		}
		catch(Util::StreamException&)
		{
			return false;
		}

		// Load from stream
		bool result = Load(stream);

		// Clean up and return
		delete stream;

		return result;
	}
}
