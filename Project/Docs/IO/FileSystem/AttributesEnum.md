## enum Attributes ##

### Description ###
	enum Attributes
This enum is used to describe the attributes of a file or directory.

**Note**: This enum can be used as flags chained together by bitwise ORing them.

### Values ###
| | Member Name    | Description                                        |
|-|--------------- | ---------------------------------------------------|
| | ReadOnly       | Read only file or directory.                       |
| | Hidden         | Hidden file or directory.                          |
| | System         | System file or directory.                          |
| | Directory      | This entry is a directory.                         |
| | Archive        | Marked to be backed up by older backup software.   |