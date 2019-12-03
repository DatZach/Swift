## property Filename ##

### Description ###
	string Filename { get; private set; }
This read-only property returns the filename that was passed to the constructor. If this contained a fullpath, then this property will return the full path of the file. Otherwise it will just be the relative path or just the filename.