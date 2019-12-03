## Constructor ##

### Description ###
	method(string filename, BinaryFile.Mode mode)
The constructor method will attempt to open a file specified in `filename` under the operation mode specified in `mode`. If a file cannot be opened the [Opened](/IO/BinaryFile/Opened) flag will be set to false, otherwise it will be true until [Close](/IO/BinaryFile/Close) is called.

### Parameter List ###
#### filename ####
> Path to file. This can be relative or absolute. Keep in mind the path is not canonicalized for the platform.

#### mode ####
> Specify how to open the file for operations. See: [BinaryFile.Mode](/IO/BinaryFile/ModeEnum).

### Return Values ###
This method returns `this`.