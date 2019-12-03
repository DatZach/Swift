## method GetFilename $ ##

### Description ###
	string GetFilename(string path)
This method will return the value specified in `path` with just the filename (including the extension) portion only.

### Parameter List ###
#### path ####
> The full or relative path to a file or directory. Keep in mind that paths are not canonicalized by Swift. A platform may not accept your path if you use the wrong path delimiters or reference a device incorrectly.

### Return Value ###
This method will return a string containing the value of `path` without the path.

### Examples ###
**Input to output**
```swift
method Create()
{
	// Will print "example.txt" to the debug console
	Debug.Log(FileSystem.GetFilename(@"C:\Users\Ada\Desktop\example.txt"));
}
```