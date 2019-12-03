/*
 *	texturesheets.hpp
 *	Texture Sheets
 */

#ifndef __GAME_DATA_TEXTURE_SHEETS_HPP
#define __GAME_DATA_TEXTURE_SHEETS_HPP

#include <vector>
#include <GameData/Asset.hpp>

#define GD_TAG_TEXTURE_SHEETS			GD_TAG_VALUE('T', 'X', 'S', 'T')

namespace GameData
{
	class TextureSheets : public IAsset
	{
	private:
		std::vector<Util::Stream*> sheets;

	public:
		TextureSheets();
		~TextureSheets();

		virtual void Read(Util::Stream* stream);

		size_t size() const;
		Util::Stream* operator[](size_t i) const;
	};
}

#endif
