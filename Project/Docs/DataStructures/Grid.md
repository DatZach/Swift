## class Grid ##

### Description ###
The `Grid` class' purpose is to promote proper and safe programming in Swift. This is primarily to get around a limitation of the way Swift Script treats lists. Since two or three dimensional lists are not 'safe' in Swift Script the Grid class provides a safe alternative to 2d arrays.

### Properties ###
| | Property Name | Description |
|-|-------------- | ------------|
|%g| [Width](/DataStructures/Grid/Width) | Width of the grid. |
|%g| [Height](/DataStructures/Grid/Height) | Height of the grid. |

### Methods ###
| | Method Name | Description |
|-|------------ | ------------|
| | [method(var width, var height)](/DataStructures/Grid/Constructor) | Create a new grid. |
| | [Get(var x, var y)](/DataStructures/Grid/Get) | Get a value from the grid. |
| | [Set(var x, var y, var value)](/DataStructures/Grid/Set) | Set a value from the grid. |

### Examples ###
**Simple tile system**
```swift
class Tiles;

public const TileSize = 32;
public var grid;

method(var width, var height)
{
	grid = Grid(width / TileSize + 1, height / TileSize + 1);
}

method Draw()
{
	for(var y = 0; y < grid.Height; ++y)
	{
		for(var x = 0; x < grid.Width; ++x)
		{
			if (grid.Get(x, y))
				Sprite(Textures.Tile).Draw(x * TileSize, y * TileSize);
		}
	}
}
```