## method DeleteDirectory $ ##

### Description ###
	bool DeleteDirectory(string path)
This method will delete a directory from the file system. The directory must be empty for this method to execute properly.

### Parameter List ###
#### path ####
> The full or relative path to the directory. Keep in mind that paths are not canonicalized by Swift. A platform may not accept your path if you use the wrong path delimiters or reference a device incorrectly.

### Return Value ###
This method will return `true` if the directory was successfully deleted from the file system. Otherwise, if the directory doesn't exist, there are files or sub-directories, or there were inadequate permissions to delete the directory, it will return `false`. 