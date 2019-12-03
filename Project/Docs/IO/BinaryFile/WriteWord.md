## method WriteWord ##

### Description ###
	void WriteWord(int value)
This method writes 2 bytes of `value` to the file and advances the cursor. This method will take the lower 16 bits of `value` and use that as the value to write to the file.

### Parameter List ###
#### value ####
>Value to write to the file.

### Return Values ###
This method returns `nil`.