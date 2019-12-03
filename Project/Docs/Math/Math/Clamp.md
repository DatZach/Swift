## method Clamp $ ##

### Description ###
	double Clamp(number x, number min, number max)
Returns the value of `x` clamped between `min` and `max`. If `x` is less than `min` then `min` is returned. If `x` is larger than `max` than `max` is returned.

### Parameter List ###
#### x ####
> Value to clamp.

#### min ####
> Minimum range to clamp.

#### max ####
> Maximum range to clamp.

### Return Values ###
The result of the operation.

### Examples ###
**Keep player inside the bounds of the view**
```swift
x = Math.Clamp(x, 0, Window.Width);
y = Math.Clamp(y, 0, Window.Height);
```