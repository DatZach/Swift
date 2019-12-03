## method WriteDword ##

### Description ###
	void WriteDword(int value)
This method writes 4 bytes of `value` to the file and advances the cursor. This method will take the lower 32 bits of `value` and use that as the value to write to the file.

### Parameter List ###
#### value ####
>Value to write to the file.

### Return Values ###
This method returns `nil`.