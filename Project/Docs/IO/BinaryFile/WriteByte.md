## method WriteByte ##

### Description ###
	void WriteByte(int value)
This method writes 1 byte of `value` to the file and advances the cursor. This method will take the lower 8 bits of `value` and use that as the value to write to the file.

### Parameter List ###
#### value ####
>Value to write to the file.

### Return Values ###
This method returns `nil`.