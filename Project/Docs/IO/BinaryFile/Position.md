## property Position ##

### Description ###
	int Position { get; set; }
This read/write property allows you to get and set the current position in bytes the cursor is at in the file. Setting the position past the end of the file or to a negative number will clamp the position to the bounds of the file size.