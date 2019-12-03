## method ReadLine ##

### Description ###
	string ReadLine()
This method returns a string containing the value of one line in the file starting from the current cursor's position. This method is platform independent and will read new line markers for both Windows (CR+LF), *NIX systems (LF) and everything in between. The new line markers will not be included in the returned string. Cursor is positioned after the new line marker.

### Parameter List ###
>*None*

### Return Values ###
A string containing a line of text read from the file.