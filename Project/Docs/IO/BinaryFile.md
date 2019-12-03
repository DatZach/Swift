## class BinaryFile ##

### Description ###
The `BinaryFile` class provides an interface for reading and writing binary files to and from a non-volatile media (such as a hard drive or sdcard). This can be used for storing data such as map files that are in a structured format and need to be tightly packed. All files read using `BinaryFile` are read in [little endian](http://en.wikipedia.org/wiki/Endianness).

For working with text files see [`TextFile`](/IO/TextFile).

### Properties ###
| | Property Name | Description |
|-|-------------- | ------------|
|%g%s| [Position](/IO/BinaryFile/Position) | Current position in the file. |
|%g| [Size](/IO/BinaryFile/Size) | Size in bytes of the file. |
|%g| [Filename](/IO/BinaryFile/Filename) | Filename of the currently opened file. |
|%g| [Mode](/IO/BinaryFile/Mode) | Mode the file is currently opened in. |
|%g| [Opened](/IO/BinaryFile/Opened) | Success status of opening the file. |
|%g| [Eof](/IO/BinaryFile/Eof) | End of File flag. |

### Methods ###
| | Method Name | Description |
|-|------------ | ------------|
| | [method(var filename, var mode)](/IO/BinaryFile/Constructor) | Open the file. |
| | [Close()](/IO/BinaryFile/Close) | Closes the file. |
| | [Seek(var offset, var origin)](/IO/BinaryFile/Seek) | Seeks to a position in the file. |
| | [ReadByte()](/IO/BinaryFile/ReadByte) | Reads 1 byte from a file. |
| | [ReadWord()](/IO/BinaryFile/ReadWord) | Reads 2 bytes from a file. |
| | [ReadDword()](/IO/BinaryFile/ReadDword) | Reads 4 bytes from a file. |
| | [ReadQword()](/IO/BinaryFile/ReadQword) | Reads 8 bytes from a file. |
| | [ReadSingle()](/IO/BinaryFile/ReadSingle) | Reads a single precision float from a file (4 bytes). |
| | [ReadDouble()](/IO/BinaryFile/ReadDouble) | Reads a double precision float from a file (8 bytes). |
| | [ReadString()](/IO/BinaryFile/ReadString) | Reads a string from a file. |
| | [WriteByte(var value)](/IO/BinaryFile/WriteByte) | Writes 1 byte to a file. |
| | [WriteWord(var value)](/IO/BinaryFile/WriteWord) | Writes 2 bytes to a file. |
| | [WriteDword(var value)](/IO/BinaryFile/WriteDword) | Writes 4 bytes to a file. |
| | [WriteQword(var value)](/IO/BinaryFile/WriteQword) | Writes 8 bytes to a file. |
| | [WriteSingle(var value)](/IO/BinaryFile/WriteSingle) | Writes a single precision float to a file (4 bytes). |
| | [WriteDouble(var value)](/IO/BinaryFile/WriteDouble) | Writes a double precision double to a file (8 bytes). |
| | [WriteString(var value)](/IO/BinaryFile/WriteString) | Writes a string to a file. |

### Enumerations ###
| | Enum Name | Description |
|-|---------- | ------------|
| | [Mode](/IO/BinaryFile/ModeEnum) | Operation mode. |
| | [Origin](/IO/BinaryFile/OriginEnum) | Seek origin. |

### Examples ###
**Saving and loading game preferences**
```swift
class GamePreferences;

private const PreferencesFilename = "preferences.dat";
private const PreferencesMagic = 1234321;

public static var FullScreen;
public static var ResolutionWidth;
public static var ResolutionHeight;
public static var Difficulty;

public static method Load()
{
	// Open the file in read mode
	var f = BinaryFile(PreferencesFilename, BinaryFile.Mode.Read);
	if (!f.Opened)
		return false;
	
	// Verify this is a setting file
	if (f.ReadDword() != PreferencesMagic)
		return false;
	
	// Load graphics settings
	FullScreen       = f.ReadByte();
	ResolutionWidth  = f.ReadWord();
	ResolutionHeight = f.ReadWord();
	
	// Load gameplay settings
	Difficulty       = f.ReadByte();
	
	// We must make sure to free our resources
	f.Close();
	
	return true;
}

public static method Save()
{
	// Open the file in write mode
	var f = BinaryFile(PreferencesFilename, BinaryFile.Mode.Write);
	if (!f.Opened)
		return false;
	
	// Save our magic number to tell us this is a settings file
	f.WriteDword(PreferencesMagic);
	
	// Write graphics settings
	f.WriteByte(FullScreen);
	f.WriteWord(ResolutionWidth);
	f.WriteWord(ResolutionHeight);
	
	// Write gameplay settings
	f.WriteByte(Difficulty);
	
	// We must make sure to free our resources.
	// This will also flush the data we wrote to
	// our file and commit it to disk.
	f.Close();
	
	return true;
}
```