## method WriteSingle ##

### Description ###
	void WriteSingle(double value)
This method writes a single precision floating point number (4 bytes) of `value` to the file and advances the cursor. This method will **cast** from double to float and use that as the value to write to the file.

### Parameter List ###
#### value ####
>Value to write to the file.

### Return Values ###
This method returns `nil`.