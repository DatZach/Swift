/*
 *	sheetcache.hpp
 *	Texture Sheets
 */

#ifndef __ENGINE_TEXTURE_SHEETS_HPP
#define __ENGINE_TEXTURE_SHEETS_HPP

#include <vector>
#include <Video/Texture.hpp>
#include <GameData/TextureSheets.hpp>

#define SheetCache		Video::SheetCacheSingleton::GetInstance()

namespace Video
{
	class SheetCacheSingleton
	{
	private:
		std::vector<Texture*> textureSheets;

	public:
		SheetCacheSingleton();
		~SheetCacheSingleton();

		void LoadSheets(const GameData::TextureSheets& sheets);

		size_t size() const;
		Texture* operator[](size_t i) const;

		static SheetCacheSingleton& GetInstance();
	};
}

#endif
