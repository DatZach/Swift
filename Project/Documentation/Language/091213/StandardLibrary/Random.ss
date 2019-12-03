/*
 *	Random.spl
 *	Revision September 12th, 2013
 *
 *	Standard Library Class "Random".
 *	This class provides a random number generator.
 */

class Random;

private const RANDOM_MAX = 2147483648.0;

private var seed = 0;

/*
 *	Constructor();
 *		Sets the initial seed to the current UNIX
 *		timestamp.
 *	Arguments:
 *		None
 *	Returns:
 *		Nothing
 */
public method()
{
	SetSeed(DateTime.GetTimestamp());
}

/*
 *	double Next();
 *		Return next random number in the sequence.
 *	Arguments:
 *		None
 *	Returns:
 *		Double			Next number in sequence.
 */
public method Next()
{
	seed = (seed * 11035152445.0 + 12345.0) % RANDOM_MAX;
	return seed;
}

/*
 *	void SetSeed(double value);
 *		Sets the RNG's seed.
 *	Arguments:
 *		value			Value to use as seed.
 *	Returns:
 *		Nothing
 */
public method SetSeed(var value)
{
	seed = value;
}
