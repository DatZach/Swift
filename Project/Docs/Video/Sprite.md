## class Sprite ##

### Description ###
The `Sprite` class provides an interface for drawing sprites on screen.

### Fields ###
| | Field Name  | Description                                   |
|-|------------ | ----------------------------------------------|
| | [x](/Video/Sprite/x) | X-Coordinate in the state.           |
| | [y](/Video/Sprite/y) | Y-Coordinate in the state.           |
| | [Animated](/Video/Sprite/Animated) | Flags animation being played. |
| | [FrameSpeed](/Video/Sprite/FrameSpeed) | Speed at which to play animation. |
| | [Color](/Video/Sprite/Color) | Blend color.                 |
| | [ScaleX](/Video/Sprite/ScaleX) | X Scale of sprite.         |
| | [ScaleY](/Video/Sprite/ScaleY) | Y Scale of sprite.         |
| | [Rotation](/Video/Sprite/Rotation) | Rotation of sprite in degrees. |

### Methods ###
| | Methods Name  | Description                                   |
|-|-------------- | ----------------------------------------------|
| | [method(var asset)](/Video/Sprite/Constructor) | Default constructor. |
| | [Draw()](/Video/Sprite/Draw) | Draw a sprite with all default attributes. |
| | [Draw(var x, var y)](/Video/Sprite/DrawXY) | Draw a sprite at (x, y). |

### Examples ###
**Draw an overlay sprite when the game is paused**
```swift
state PausedExample;

method Draw()
{
	// Assume that the origin is center
	if (!Active)
		Sprite(Textures.PausedOverlay).Draw(Window.Width / 2, Window.Height / 2);
}

method OnFocusChanged(var value)
{
	Active = value;
}
```