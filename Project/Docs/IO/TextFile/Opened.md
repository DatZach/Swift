## property Opened ##

### Description ###
	bool Opened { get; private set; }
This read-only property flags if the constructor successfully opened a handle to the specified file. If this is false you should not try and attempt any operations as they will fail silently.