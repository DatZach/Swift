## property WorkingDirectory $ ##

### Description ###
	string WorkingDirectory { get; private set; }
This read-only property returns the full current working directory. The current working directory will be the directory from which the game is ran from.

### Value ###
As an example, if the working directory was a directory on the user 'Ada's desktop named 'MyGame' then `WorkingDirectory` would be reported as so on the following platforms:

**Windows**

	C:\Users\Ada\Desktop\MyGame

**Mac OS X**

	/Users/Ada/Desktop/MyGame

**Linux**

	/home/Ada/Desktop/MyGame