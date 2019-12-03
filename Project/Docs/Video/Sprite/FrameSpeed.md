## field FrameSpeed ##

### Description ###
	double FrameSpeed
This field describes the speed at which the frames change. By default this field is initialized to 1.0. Every time [`Draw()`](/Video/Sprite/Draw) is called the current `FrameSpeed` will be added to an internal frame counter. The whole number of that frame counter will be used as the current frame.

The algorithm:

	((int)(currentFrame += FrameSpeed) % totalFrames).

is used to determine the current frame to draw.

If the `FrameSpeed` is 0.1, for instance, it will take 10 frames for the sprite to switch to the next frame.

A negative `FrameSpeed` means that the animation will play backwards.