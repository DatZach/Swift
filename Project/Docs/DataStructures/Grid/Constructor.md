## method Constructor ##

### Description ###
	method(number width, number height)
The constructor method will allocate a 2d array internally of the size (`width`, `height`). The properties [Width](/DataStructures/Grid/Width) and [Height](/DataStructures/Grid/Height) are set to the value of `width` and `height` respectively. Memory will be freed when there are no longer any references to the grid and the Garbage Collector decides to make a pass.

### Parameter List ###
#### width ####
> Width of 2d array. Can be a double but will lose precision due to casting.

#### height ####
> Height of 2d array. Can be a double but will lose precision due to casting.

### Return Values ###
This method returns `this`.