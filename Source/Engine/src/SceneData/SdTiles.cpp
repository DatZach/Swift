/*
 *	sdtiles.cpp
 *	Scene Data Tiles
 */

#include <SceneData/Tiles.hpp>

namespace SceneData
{
	Tiles::TileMap::Tile::Tile()
		: ISection(),
		  position(),
		  tilePosition()
	{

	}

	void Tiles::TileMap::Tile::Read(Util::Stream* stream)
	{
		position.x		= stream->ReadWord();
		position.y		= stream->ReadWord();
		tilePosition.x	= stream->ReadByte();
		tilePosition.y	= stream->ReadByte();
	}

	Tiles::TileMap::TileMap()
		: ISection(),
		  textureName(""),
		  tileSize(),
		  tiles()
	{

	}

	void Tiles::TileMap::Read(Util::Stream* stream)
	{
		textureName = stream->ReadString();
		tileSize.x	= stream->ReadWord();
		tileSize.y	= stream->ReadWord();

		unsigned int count = stream->ReadDword();
		while(count--)
		{
			Tile tile;
			tile.Read(stream);

			tiles.push_back(tile);
		}
	}

	Tiles::Tiles()
		: ISection(),
		  maps()
	{

	}

	void Tiles::Read(Util::Stream* stream)
	{
		unsigned int count = stream->ReadDword();
		while(count--)
		{
			TileMap map;
			map.Read(stream);

			maps.push_back(map);
		}
	}

	size_t Tiles::size() const
	{
		return maps.size();
	}

	const Tiles::TileMap& Tiles::operator[](size_t i) const
	{
		return maps[i];
	}
}
