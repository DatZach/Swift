## method FileExists $ ##

### Description ###
	int FileExists(string filename)
This method returns if a file specified in `filename` exists.

### Parameter List ###
#### filename ####
> The full or relative path including the filename with the extension. Keep in mind that paths are not canonicalized by Swift. A platform may not accept your path if you use the wrong path delimiters or reference a device incorrectly.

### Return Values ###
This method will return `true` if the file exists. Otherwise it will return `false`.