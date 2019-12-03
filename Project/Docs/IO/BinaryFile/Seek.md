## method Seek ##

### Description ###
	void Seek(int offset, BinaryFile.Origin origin)
This method will move the cursor's position in the stream by `offset` amount from the `origin` point. Seeking before the start of the file or past the end of the file will clamp the cursor's position to be inside of the size of the file.

**Note**: It is recommended that you use the [`Position`](/IO/BinaryFile/Position) property instead of this method.

### Parameter List ###
#### offset ####
>The amount in bytes to seek relative to the origin.

#### origin ####
>The [`Origin`](/IO/BinaryFile/Origin) point to start seeking from.

### Returns ###
This method returns `nil`.