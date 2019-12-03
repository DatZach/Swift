## enum FileMode ##

### Description ###
	enum FileMode
This enum is used to describe the filter mode for the entries to be returned by [`GetFiles`](/IO/FileSystem/GetFiles).

**Note**: This enum can be used as flags chained together by bitwise ORing them.

### Values ###
| | Member Name    | Description                                        |
|-|--------------- | ---------------------------------------------------|
| | Files          | Include files in the resulting list.               |
| | Directories    | Include directories in the resulting list.         |
| | Devices        | Include devices (disks or partitions) in the resulting list. | 