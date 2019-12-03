## property Volume ##

### Description ###
```swift
double Volume { get; set; }
```
Volume is a read-write property that describes the volume of the sound when played. This property is a double clamped between 0.0 and 1.0 (0 - 100). This value can be changed while the sound is playing allowing for fade in and out effects.