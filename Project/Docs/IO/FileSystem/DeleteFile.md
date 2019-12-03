## method DeleteFile $ ##

### Description ###
	bool DeleteFile(string filename)
This method will delete a file from the file system.

### Parameter List ###
#### filename ####
> The full or relative path to the file including the filename and extension. Keep in mind that paths are not canonicalized by Swift. A platform may not accept your path if you use the wrong path delimiters or reference a device incorrectly.

### Return Value ###
This method will return `true` if the file was successfully deleted from the file system. Otherwise, if the file doesn't exist or there were inadequate permissions to delete the file, it will return `false`. 