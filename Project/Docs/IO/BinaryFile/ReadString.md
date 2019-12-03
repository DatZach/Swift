## method ReadString ##

### Description ###
	string ReadString()
This method will read a string from the file and advance the cursor. Strings are read from the file by reading a [7-bit encoded integer](http://en.wikipedia.org/wiki/Variable-length_quantity) header which contains the length of the string. The string is then read. Strings are **not** null terminated in the file.

### Parameter List ###
>*None*

### Return Values ###
The value of the data read from the file.