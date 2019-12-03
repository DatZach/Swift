## method Close ##

### Description ###
	void Close()
This method closes the handle to the file. If the file was in write [mode](/IO/TextFile/Mode) then all changes will be flushed to the media.

**Note**: It is important to call this when you are done working with a file. Swift's Garbage Collector will close the file before freeing the memory but this can happen at any time. It's better to explicitly close the file yourself to avoid potential bugs relating to the file being locked if you try to reaccess it again soon after.

### Parameters ###
>*None*

### Return Values ###
This method returns `nil`.