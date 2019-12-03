## method Abs $ ##

### Description ###
	number Abs(number x)
Returns the [absolute value](http://en.wikipedia.org/wiki/Absolute_Value) of `x`. This method will retain the type when returning. If `x` is a double, a double will be returned. If it is an integer then an integer will be returned.

### Parameter List ###
#### x ####
> Value

### Return Values ###
Returns |`x`|. If `x` is a double, return type will be a double. If `x` is an integer, return type will be integer. If `x` is neither a double or an int then nil will be returned and an error will be printed to the debug console.