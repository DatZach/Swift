## enum Mode ##

### Description ###
	enum Mode
This enumeration describes the mode in which the file is opened for operations.

**Note**: This enum can be used as flags chained together by bitwise ORing them.

### Values ###
| | Member Name    | Description                                        |
|-|--------------- | ---------------------------------------------------|
| | Read           | File will be opened for reading.                   |
| | Write          | File will be opened for writing.                   |
| | Truncate       | All contents will be discarded before opening. Opened for writing.     |
| | Append         | Cursor is positioned at end of file for appending. |