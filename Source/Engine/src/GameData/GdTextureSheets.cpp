/*
 *	gdtexturesheets.cpp
 *	Texture Sheets
 */

#include <GameData/TextureSheets.hpp>

namespace GameData
{
	TextureSheets::TextureSheets()
		: IAsset(),
		  sheets()
	{

	}

	TextureSheets::~TextureSheets()
	{
		for(Util::Stream* sheet : sheets)
			delete sheet;
	}

	void TextureSheets::Read(Util::Stream* stream)
	{
		unsigned int count = stream->ReadDword();
		while(count--)
			sheets.push_back(stream->Deserialize(false));
	}

	size_t TextureSheets::size() const
	{
		return sheets.size();
	}

	Util::Stream* TextureSheets::operator[](size_t i) const
	{
		return sheets[i];
	}
}
