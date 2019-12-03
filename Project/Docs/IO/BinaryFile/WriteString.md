## method WriteString ##

### Description ###
	void WriteString(string value)
This method writes a string of `value` to the file and advances the cursor. A [7-bit encoded integer](http://en.wikipedia.org/wiki/Variable-length_quantity) header is written containing the length of the string first, then a non-null-terminated string is written.

### Parameter List ###
#### value ####
>Value to write to the file.

### Return Values ###
This method returns `nil`.