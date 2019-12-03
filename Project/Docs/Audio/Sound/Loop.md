## method Loop ##

### Description ###
```swift
void Loop()
```
This method will change the [playing state](/Audio/Sound/StatusEnum) of the sound to playing. The sound will seek to 0ms and continue playing when it finishes playing until [`Stop`](/Audio/Sound/Stop) is called. This will always reset the position of the sound to 0ms before playing.

### Parameters ###
>*None*

### Return Values ###
This method will always return nil.