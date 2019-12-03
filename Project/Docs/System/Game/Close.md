## method Close $ ##

### Description ###
	void Close()
This method will attempt to close the game and shutdown the Engine. Note that this method will only close the Engine if all states on the current state stack return true in their 'ExitRequested' method.

### Parameter List ###
> *None*

### Return Values ###
This method returns `nil`.