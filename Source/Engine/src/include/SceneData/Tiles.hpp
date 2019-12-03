/*
 *	tiles.hpp
 *	Scene Data Tiles
 */

#ifndef __SCENE_DATA_TILES_HPP
#define __SCENE_DATA_TILES_HPP

#include <vector>
#include <string>
#include <SceneData/Section.hpp>
#include <Util/Vector2.hpp>

#define SD_TAG_TILES			SD_TAG_VALUE('T', 'I', 'L', 'E')

namespace SceneData
{
	class Tiles : public ISection
	{
	public:
		class TileMap : public ISection
		{
		public:
			class Tile : public ISection
			{
			public:
				Vector2u	position;
				Vector2u	tilePosition;

			public:
				Tile();

				virtual void Read(Util::Stream* stream);
			};

		public:
			std::string			textureName;
			Vector2u			tileSize;
			std::vector<Tile>	tiles;

		public:
			TileMap();

			virtual void Read(Util::Stream* stream);
		};

	private:
		std::vector<TileMap>	maps;

	public:
		Tiles();

		virtual void Read(Util::Stream* stream);

		size_t size() const;
		const TileMap& operator[](size_t i) const;
	};
}

#endif
