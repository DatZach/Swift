## method GetExtension $ ##

### Description ###
	string GetExtension(string path)
This method will return the value specified in `path` with just the file's extension portion only.

### Parameter List ###
#### path ####
> The full or relative path to a file. Keep in mind that paths are not canonicalized by Swift. A platform may not accept your path if you use the wrong path delimiters or reference a device incorrectly.

### Return Value ###
This method will return a string containing the value of `path` with just the extension. No '.' will be included.

### Examples ###
**Input to output**
```swift
method Create()
{
	// Will print "txt" to the debug console
	Debug.Log(FileSystem.GetExtension(@"C:\Users\Ada\Desktop\example.txt"));
}
```