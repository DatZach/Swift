## method Choose $ ##

### Description ###
	var Choose(var a, var b);
This method will randomly choose `a` or `b` given an equal probability and return the chosen value. This method will accept all types of variant and return whatever was passed to it.

### Parameter List ###
#### a ####
> First value to choose from.

#### b ####
> Second value to choose from.

### Return Values ###
Either `a` or `b`.

### Examples ###
**Choosing between a positive or negative number**
```swift
private const Speed = 4;

var speed = Random.Choose(Speed, -Speed);
```

**Choosing between two victory messages**
```swift
var message = Random.Choose("Good job, you did it!", "Nice!");
```