## class Debug ##

### Description ###
The `Debug` class provides an interface to the 'Engine Debug' window, allowing you to output various debug information. Nothing printed from the `Debug` class will be visible in the release build of the game and can only be seen when ran in debug from the Swift IDE.

### Methods ###
| | Method Name | Description |
|-|------------ | ------------|  
|$| [WriteLine](/System/Debug/WriteLine) | Writes a line of text to the debug console. |
|$| [Write](/System/Debug/Write) | Writes to the debug console. |
|$| [Log](/System/Debug/Log) | Writes to the debug console using 'Log'. |
|$| [Trace](/System/Debug/Trace) | Writes to the debug console using 'Trace'. |
|$| [Warn](/System/Debug/Warn) | Writes to the debug console using 'Warn'. |
|$| [Error](/System/Debug/Error) | Writes to the debug console using 'Error'. |

### Examples ###
**Log loading progress**
```swift
method Create()
{
	Debug.Log("Starting level load...");
	Debug.Log("Loading scene objects...");
	LoadSceneObjects();

	Debug.Log("Loading scene...");
	LoadScene(Scenes.Example);

	Debug.Log("Settling scene objects...");
	SettleSceneObjects();

	Debug.Log("Done!");
}
```