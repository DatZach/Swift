## class TextFile ##

### Description ###
The `TextFile` class provides an interface for reading and writing text files to and from a non-volatile media (such as a hard drive or sdcard). This is useful for storing data that needs to be human readable for one reason or another. Text files are read as if they are encoded with [UTF-8](http://en.wikipedia.org/wiki/UTF-8).

For working with binary files see [`BinaryFile`](/IO/BinaryFile).

### Properties ###
| | Property Name | Description |
|-|-------------- | ------------|
|%g%s| [Position](/IO/TextFile/Position) | Current position in the file. |
|%g| [Size](/IO/TextFile/Size) | Size in bytes of the file. |
|%g| [Filename](/IO/TextFile/Filename) | Filename of the currently opened file. |
|%g| [Mode](/IO/TextFile/Mode) | Mode the file is currently opened in. |
|%g| [Opened](/IO/TextFile/Opened) | Success status of opening the file. |
|%g| [Eof](/IO/TextFile/Eof) | End of File flag. |

### Methods ###
| | Method Name | Description |
|-|------------ | ------------|
| | [method(var filename, var mode)](/IO/TextFile/Constructor) | Open the file. |
| | [Close()](/IO/TextFile/Close) | Closes the file. |
| | [Seek(var offset, var origin)](/IO/TextFile/Seek) | Seeks to a position in the file. |
| | [Read(var length)](/IO/TextFile/Read) | Reads text from a file. |
| | [ReadLine()](/IO/TextFile/ReadLine) | Reads a line of text from a file. |
| | [ReadToEnd()](/IO/TextFile/ReadToEnd) | Reads text from current position to end of file. |
| | [Write(var value)](/IO/TextFile/Write) | Writes text to a file. |
| | [WriteLine(var value)](/IO/TextFile/WriteLine) | Writes a line of text to a file. |

### Enumerations ###
| | Enum Name | Description |
|-|---------- | ------------|
| | [Mode](/IO/TextFile/ModeEnum) | Operation mode. |
| | [Origin](/IO/TextFile/OriginEnum) | Seek origin. |

### Examples ###
**Loading game credits**
```swift
state GameCreditScreen;

private var creditsValue;

public method Create()
{
	// Open the credits file
	var f = TextFile("credits.txt", TextFile.Mode.Read);
	if (f.Opened)
	{
		// Read everything
		creditsValue = f.ReadToEnd();

		// Remember to free our resources
		f.Close();
	}
}
```