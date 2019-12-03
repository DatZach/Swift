## method WriteLine ##

### Description ###
	void WriteLine(string value)
This method writes a string of `value` to the file and then writes a platform dependent new line marker. The new line marker written depends on the platform Swift is currently running on. Swift running on Windows will write a CR+LF and on *NIX systems LF will be written.

### Parameter List ###
#### value ####
> The string to write to the file.

### Return Values ###
This method returns `nil`.