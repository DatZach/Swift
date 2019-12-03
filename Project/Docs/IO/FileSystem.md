## class FileSystem ##

### Description ###
The `FileSystem` class provides an abstraction to the filesystem of the platform the Engine is running on.

### Properties ###
| | Property Name | Description |
|-|-------------- | ------------|
|$%g| [WorkingDirectory](/IO/FileSystem/WorkingDirectory) | Returns the current working directory. |
|$%g| [TempDirectory](/IO/FileSystem/TempDirectory) | Returns the system's temporary directory. |

### Methods ###
| | Method Name | Description |
|-|------------ | ------------|
|$| [FileExists(var filename)](/IO/FileSystem/FileExists) | Checks if a file exists. |
|$| [DirectoryExists(var path)](/IO/FileSystem/DirectoryExists) | Checks if a directory exists. |
|$| [DeleteFile(var filename)](/IO/FileSystem/DeleteFile) | Deletes a file from the file system. |
|$| [DeleteDirectory(var path)](/IO/FileSystem/DeleteDirectory) | Deletes a directory from the file system. |
|$| [GetFiles(var filter, var attributes)](/IO/FileSystem/GetFiles) | Returns a list of files. |
|$| [GetAttributes(var filename)](/IO/FileSystem/GetAttributes) | Get the attributes of a file. |
|$| [GetFilename(var filename)](/IO/FileSystem/GetFilename) | Return the filename without path. |
|$| [GetExtension(var filename)](/IO/FileSystem/GetExtension) | Return the extension of a filename. |
|$| [GetPath(var filename)](/IO/FileSystem/GetPath) | Return the path without the filename. |

### Enumerations ###
| | Enum Name | Description |
|-|---------- | ------------|
| | [FileMode](/IO/FileSystem/FileModeEnum) | Kinds of files to find. |
| | [Attributes](/IO/FileSystem/AttributesEnum) | File attributes. |
