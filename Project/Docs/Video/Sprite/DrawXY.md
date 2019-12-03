## method Draw ##

### Description ###
	void Draw(number x, number y)
This method will draw the sprite with all defaults used except for the fields [`x`](/Video/Sprite/x) and [`y`](/Video/Sprite/y). That means it will be drawn at the position specified by the parameters (`x`, `y`) with the [blend color](/Video/Sprite/Color), [x-scale](/Video/Sprite/ScaleX), [y-scale](/Video/Sprite/ScaleY) and [rotation](/Video/Sprite/Rotation) being taken into account.

This method will also update the animation information as described in [`FrameSpeed`](/Video/Sprite/FrameSpeed).

### Parameter List ###
#### x ####
> X-Coordinate to draw the sprite at.

#### y ####
> Y-Coordinate to draw the sprite at.

### Return Value ###
This method returns `nil`.