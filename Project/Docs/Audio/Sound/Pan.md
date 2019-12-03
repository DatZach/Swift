## property Pan ##

### Description ###
```swift
double Pan { get; set; }
```
Pan is a read/write property that describes the pan of the sound when played. This property is a double clamped between -1.0 and 1.0 (-100 - 100). This value can be changed while the sound is playing allowing positional sound effects.