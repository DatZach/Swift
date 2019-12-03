/*
 *	sheetcache.cpp
 *	Texture Sheets
 */

#include <string>
#include <sstream>
#include <Video/SheetCache.hpp>
#include <Video/Video.hpp>
#include <Util/Logger.hpp>
#include <Util/StopWatch.hpp>

namespace Video
{
	SheetCacheSingleton::SheetCacheSingleton()
		: textureSheets()
	{

	}

	SheetCacheSingleton::~SheetCacheSingleton()
	{
		for(Texture* sheet : textureSheets)
			delete sheet;
	}

	void SheetCacheSingleton::LoadSheets(const GameData::TextureSheets& sheets)
	{
		for(size_t i = 0; i < sheets.size(); ++i)
		{
			Util::Stopwatch timer;
			timer.Start();

			Texture* texture = new Texture();
			if (!texture->LoadStream(sheets[i]))
			{
				std::stringstream msg;
				msg << "Cannot load texture sheet of index "<< i << ".";

				throw VideoException(msg.str());
			}

			textureSheets.push_back(texture);

			timer.Stop();

			Trace << "Loaded texture sheet in " << timer.GetElapsedMilliseconds() << "ms." << lendl;
		}
	}

	inline size_t SheetCacheSingleton::size() const
	{
		return textureSheets.size();
	}

	Texture* SheetCacheSingleton::operator[](size_t i) const
	{
#ifdef DEBUG
		if (i >= size())
		{
			Warn << "Attempted to access non-existant cached texture sheet " << i << "." << lendl;
			return NULL;
		}
#endif

		return textureSheets[i];
	}

	SheetCacheSingleton& SheetCacheSingleton::GetInstance()
	{
		static SheetCacheSingleton instance;
		return instance;
	}
}
