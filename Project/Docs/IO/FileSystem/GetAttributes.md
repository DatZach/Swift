## method GetAttributes $ ##

### Description ###
	FileSystem.Attributes GetAttributes(string path)
This method will return the attributes as reported by the Operating System of whatever `path` contains. `path` can be a relative or full path and can point to a file or a directory.

### Parameter List ###
#### path ####
> Path to the file or directory to return the attributes of. Keep in mind that paths are not canonicalized by Swift. A platform may not accept your path if you use the wrong path delimiters or reference a device incorrectly.

### Return Value ###
This method will returns an integer containing the flags specified in the enumeration [`FileSystem.Attributes`](/IO/FileSystem/AttributesEnum).