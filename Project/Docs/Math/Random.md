## class Random ##

### Description ###
The `Random` class provides an interface for generating random numbers. The PRNG is dependent on the platform Swift is currently running on since Swift just uses the underlying system's random implementation. When Swift initializes, the current time (using the [UNIX timestamp](http://en.wikipedia.org/wiki/Unix_time)) is used as an initial seed for the RNG.

### Methods ###
| | Method Name   | Description |
|-|-------------- | ------------|
|$| [SetSeed(var value)](/Math/Random/SetSeed) | Set the seed to use for the RNG. |
|$| [Next()](/Math/Random/Next) | Returns a random number. |
|$| [Next(var max)](/Math/Random/NextMax) | Returns a random number limited to below max. |
|$| [Next(var min, var max)](/Math/Random/NextMinMax) | Returns a random number between min and max. |
|$| [Choose(var a, var b)](/Math/Random/Choose) | Returns either a or b randomly. |