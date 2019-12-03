## method GetPath $ ##

### Description ###
	string GetPath(string path)
This method will return the value specified in `path` with just the path portion only.

### Parameter List ###
#### path ####
> The full or relative path to a file. Keep in mind that paths are not canonicalized by Swift. A platform may not accept your path if you use the wrong path delimiters or reference a device incorrectly.

### Return Value ###
This method will return a string containing the value of `path` with just the path, no filename or extension will be included. No trailing backslash will be included.

### Examples ###
**Input to output**
```swift
method Create()
{
	// Will print "C:\Users\Ada\Desktop" to the debug console
	Debug.Log(FileSystem.GetPath(@"C:\Users\Ada\Desktop\example.txt"));
}
```