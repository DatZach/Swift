/*
 *	tilemap.hpp
 *	Tile Map
 */

#ifndef __VIDEO_TILE_MAP_HPP
#define __VIDEO_TILE_MAP_HPP

#include <vector>
#include <Video/VertexBuffer.hpp>
#include <Video/Texture.hpp>
#include <Video/Drawable.hpp>
#include <GameData/Textures.hpp>

namespace Video
{
	class TileMap
	{
	private:
		GameData::Texture					gdTexture;
		std::vector<std::vector<Vector2u> >	map;
		Vector2u							tileSize;
		Vector2u							mapSize;

	public:
		TileMap();

		void SetTexture(const std::string& value);
		void SetTileSize(const Vector2u& value);
		void SetMapSize(const Vector2u& value);
		void Add(const Vector2u& pos, const Vector2u& tile);
		void Clear();

		void Draw();

		bool PlaceFree(const Vector2u& position) const;
	};
}

#endif
