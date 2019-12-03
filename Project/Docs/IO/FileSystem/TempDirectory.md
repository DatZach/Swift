## property TempDirectory $ ##

### Description ###
	string TempDirectory { get; private set; }
This read-only property returns the system's preferred temporary directory that should be used for temporary files and directories.

### Value ###
On the following platforms the temporary directory might be reported as:

**Windows**

	C:\Users\<CurrentUser>\AppData\Temp

**Mac OS X**

	/tmp

**Linux**

	/tmp