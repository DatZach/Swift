## method DirectoryExists $ ##

### Description ###
	bool DirectoryExists(string path)
This method returns if a directory specified in `path` exists.

### Parameter List ###
#### filename ####
> The full or relative path. Keep in mind that paths are not canonicalized by Swift. A platform may not accept your path if you use the wrong path delimiters or reference a device incorrectly.

### Return Values ###
This method will return `true` if the file exists. Otherwise it will return `false`.