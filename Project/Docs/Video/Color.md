## class Color ##

### Description ###
The `Color` class is used by various classes in the API (particularly the Video API) to specify the colors of various drawable objects. This class is largely just a container for colors.

### Fields ###
| | Field Name  | Description                                   |
|-|------------ | ----------------------------------------------|
| | [r](/Video/Color/r) | Red component, clamped to 0 -> 255.   |
| | [g](/Video/Color/g) | Green component, clamped to 0 -> 255. |
| | [b](/Video/Color/b) | Blue component, clamped to 0 -> 255.  |
| | [a](/Video/Color/a) | Alpha component, clamped to 0 -> 255. |

### Methods ###
| | Method Name | Description |
|-|------------ | ------------|
| | [method()](/Video/Color/Constructor) | Default constructor. Black. |
| | [method(var r, var g, var b)](/Video/Color/ConstructorRGB) | Constructor for just RGB components. |
| | [method(var r, var g, var b, var a)](/Video/Color/ConstructorRGBA) | Constructor to specify RGBA. |

### Examples ###
```swift
state ColorExample
{
	// Set the background color to a shade of blue.
	// Red = 96, Green = 128 and Blue = 192
	ClearColor = Color(96, 128, 192)
};
```