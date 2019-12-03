## method PointInRect $ ##

### Description ###
	bool PointInRect(number px, number py, number rx, number ry, number width, number height);
Returns if the point (`px`, `py`) is in the rectangle (`rx`, `ry`, `rx` + `width`, `ry` + `height`) with the border inclusively.

### Parameter List ###
#### px ####
> Point's X coordinate.

#### py ####
> Point's Y coordinate.

#### rx ####
> Rectangle's X coordinate.

#### ry ####
> Rectangle's Y coordinate.

#### width ####
> Rectangle's width.

#### height ####
> Rectangle's height.

### Return Values ###
Returns true if the point is inside of the rectangle inclusively.

### Examples ###
**Check if the mouse is over a button**
```swift
var hovering = Math.PointInRect(Mouse.X, Mouse.Y, x, y, width, height);
```