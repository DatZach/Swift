## class Sound ##

### Description ###
The sound class provides an interface for playing sounds through the Operating System's chosen playback device. This can be both background music and sound effects. Since `Sound` is a instance class sounds played are instanced.

### Properties ###
| | Property Name | Description |
|-|-------------- | ------------|
|%g%s| [double Volume](/Audio/Sound/Volume) | Volume of the sound. |
|%g%s| [double Pan](/Audio/Sound/Pan) | Pan of the sound. |
|%g%s| [double Pitch](/Audio/Sound/Pitch) | Pitch of the sound. |
|%g| [double Status](/Audio/Sound/Status) | Play status of the sound. |

### Methods ###
| | Method Name | Description |
|-|------------ | ------------|
| | [method(var assetName)](/Audio/Sound/Constructor) | Default constructor. |
| | [Play()](/Audio/Sound/Play) | Play a sound once. |
| | [Loop()](/Audio/Sound/Loop) | Continuously play a sound. |
| | [Stop()](/Audio/Sound/Stop) | Stop playing a sound. |

### Enumerations ###
| | Enum Name | Description |
|-|---------- | ------------|
| | [Status](/Audio/Sound/StatusEnum) | Play status of the song. |

### Examples ###
**Quickly play a sound once with all presets**
```swift
Sound(Sounds.MySound).Play();
```

**Play a sound once at half volume**
```swift
var mySound = Sound(Sounds.MySound);
mySound.Volume = 0.5;
mySound.Play();
```