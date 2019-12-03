## method GetFiles $ ##

### Description ###
	list GetFiles(string filter, FileSystem.FileMode mode)
This method will return a list of files without the path that were found by the `filter` and in the directory specified by `filter`. Entries will be filtered by the value set in `mode`.

### Parameter List ###
#### filter ####
> The full or relative path with the filename portion of the path being a filter. Keep in mind that paths are not canonicalized by Swift. A platform may not accept your path if you use the wrong path delimiters or reference a device incorrectly.

#### mode ####
> The entry filter mode. This will allow you to filter files, directories and devices from the returned list. See: [FileSystem.FileMode](/IO/FileSystem/FileModeEnum).

### Return Value ###
This method will return a list of strings containing the results of the operation. None of the returned names will contain the path they were found in.

### Examples ###
**Find all .png files in another directory**
```swift
method Create()
{
	// Will work on all desktop platforms
	var files = FileSystem.GetFiles("assets/textures/*.png", FileSystem.FileMode.Files);
	Debug.Log("Found textures: " + str(files));
}
```