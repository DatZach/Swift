## method Min $ ##

### Description ###
	var Min(var x, var y)
Compares `x` with `y` and returns the smallest of the two. Note that the parameter and return types are of type Variant. This is because it does a language level comparison of the arguments and returns a copy of the argument. Meaning strings, lists, objects, etc. can be passed but might result in a comparison error.

### Parameter List ###
#### x ####
> First value to compare.

#### y ####
> Second value to compare.

### Return Values ###
The smallest value of the two passed arguments. The return type is whatever the type of the argument passed was.