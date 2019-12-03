/*
 *	tilemap.cpp
 *	Tile Map
 */

#include <Engine/Game.hpp>
#include <Video/SheetCache.hpp>
#include <Video/TileMap.hpp>

namespace Video
{
	TileMap::TileMap()
		: gdTexture(),
		  map(),
		  tileSize(),
		  mapSize()
	{

	}

	void TileMap::SetTexture(const std::string& value)
	{
		// TODO Error catching
		gdTexture = *Assets.textures[value];
	}

	void TileMap::SetTileSize(const Vector2u& value)
	{
		// Argument provided in pixels
		tileSize = value;
	}

	void TileMap::SetMapSize(const Vector2u& value)
	{
		// Argument provided in pixels
		if (tileSize.x == 0 || tileSize.y == 0)
			return;

		// Value translated to tile units (indices of tiles)
		mapSize.x = value.x / tileSize.x;
		mapSize.y = value.y / tileSize.y;

		map.resize(mapSize.x);
		for(size_t i = 0; i < mapSize.x; ++i)
			map[i].resize(mapSize.y);

		for(size_t y = 0; y < mapSize.y; ++y)
		{
			for(size_t x = 0; x < mapSize.x; ++x)
				map[x][y] = Vector2u(-1, -1);
		}
	}

	void TileMap::Add(const Vector2u& pos, const Vector2u& tile)
	{
		// Position in tile units
		// Tile Position in tile units
		map[pos.x][pos.y] = Vector2u(tile.x * tileSize.x, tile.y * tileSize.y);
	}

	void TileMap::Clear()
	{
		map.clear();
		map.resize(mapSize.x);
		for(size_t i = 0; i < mapSize.x; ++i)
			map[i].resize(mapSize.y);

		for(size_t y = 0; y < mapSize.y; ++y)
		{
			for(size_t x = 0; x < mapSize.x; ++x)
				map[x][y] = Vector2u(-1, -1);
		}
	}

	void TileMap::Draw()
	{
		// Check if we set a texture
		if (gdTexture.name == "")
			return;

		GameData::Frame& frame = gdTexture.frames[0];
		Texture* textureSheet = SheetCache[frame.sheetIndex];
		textureSheet->SetLinear(false);

		Vector2i sheetSize = textureSheet->GetSize();

		for(size_t ty = 0; ty < mapSize.y; ++ty)
		{
			for(size_t tx = 0; tx < mapSize.x; ++tx)
			{
				const Vector2u& tile = map[tx][ty];
				if (tile.x == -1 || tile.y == -1)
					continue;

				VertexBuffer vertexBuffer;

				float x = (float)frame.positionX + tile.x;
				float y = (float)frame.positionY + tile.y;
				float width = (float)tileSize.x;
				float height = (float)tileSize.y;

				float tX1 = x / sheetSize.x;
				float tY1 = y / sheetSize.y;
				float tX2 = (x + width) / sheetSize.x;
				float tY2 = (y + height) / sheetSize.y;

				float positionX = (float)tx * tileSize.x;
				float positionY = (float)ty * tileSize.y;

				// Top Left
				vertexBuffer.Add(
					Vertex(
						Vector2f(
							positionX,
							positionY
						),
						Color::White,
						Vector2f(tX1, tY1)
					)
				);
		
				// Top Right
				vertexBuffer.Add(
					Vertex(
						Vector2f(
							positionX + width,
							positionY
						),
						Color::White,
						Vector2f(tX2, tY1)
					)
				);

				// Bottom Right
				vertexBuffer.Add(
					Vertex(
						Vector2f(
							positionX + width,
							positionY + height
						),
						Color::White,
						Vector2f(tX2, tY2)
					)
				);

				// Bottom Left
				vertexBuffer.Add(
					Vertex(
						Vector2f(
							positionX,
							positionY + height
						),
						Color::White,
						Vector2f(tX1, tY2)
					)
				);

				textureSheet->Bind();

				glBufferData(GL_ARRAY_BUFFER, vertexBuffer.GetSize(), vertexBuffer.GetBuffer(), GL_STATIC_DRAW);
				glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
			}
		}
	}

	bool TileMap::PlaceFree(const Vector2u& position) const
	{
		unsigned int tx = position.x / tileSize.x;
		unsigned int ty = position.y / tileSize.y;
		if (tx >= mapSize.x || ty >= mapSize.y)
			return true;
		
		const Vector2u& tile = map[tx][ty];
		
		return tile.x == -1 || tile.y == -1;
	}
}
