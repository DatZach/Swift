## method(var assetName) ##

### Description ###
	method(string assetName)
The `Sound` class constructor creates a new instance of the sound specified in `assetName`.

### Parameters ###
#### assetName ####
>String containing the symbolic name of the sound to create an instance of.

### Return Values ###
Returns a `this` pointer.

### Notes ###
The parameter `assetName` should be passed by the special namespace `Sounds` instead of as a string literal. This will help catch errors renamed to renamed assets.

**Example**
```swift
// Recommended:
var mySound = Sound(Sounds.MySound);
// Avoid doing:
var mySound = Sound("MySound");
```