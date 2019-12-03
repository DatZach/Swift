## property Eof ##

### Description ###
	bool Eof { get; private set; }
This read-only property flags if the stream is at the End of File (EOF). When writing this flag will usually be true if you're not seeking around. Read operations have undefined behavior when reading past EOF.