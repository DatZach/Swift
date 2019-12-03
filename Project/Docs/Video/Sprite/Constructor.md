## method(var assetName) ##

### Description ###
	method(string assetName)
The `Sprite` class constructor creates a new instance of the sprite with the texture  specified in `assetName`.

### Parameters ###
#### assetName ####
>String containing the symbolic name of the texture to use when creating an instance.

### Return Values ###
Returns a `this` pointer.

### Notes ###
The parameter `assetName` should be passed by the special namespace `Textures` instead of as a string literal. This will help catch errors renamed to renamed assets.

**Example**
```swift
// Recommended:
var mySprite = Sprite(Textures.Player);
// Avoid doing:
var mySprite = Sound("Player");
```