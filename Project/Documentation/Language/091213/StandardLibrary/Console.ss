/*
 *	Console.spl
 *	Revision September 12th, 2013
 *
 *	Standard Library Class "Console".
 *	This class provides access to the console if
 *	the engine is running in debug mode.
 *	Output will be redirected to IDE or another
 *	console if requested by calling parent process.
 */
 
global Console;

/*
 *	void Clear();
 *		Clear console.
 *	Arguments:
 *		None
 *	Returns:
 *		Nothing
 */
public method Clear()
{
	// Implemented via hook in Engine
}

/*
 *	void WriteLine(string value);
 *		Print line to console.
 *	Arguments:
 *		value			Value to print to console.
 *	Returns:
 *		Nothing
 */
public method WriteLine(var value)
{
	// Implemented via hook in Engine
}

/*
 *	void Write(string value);
 *		Print value to console.
 *	Arguments:
 *		value			Value to print to console.
 *	Returns:
 *		Nothing
 */
public method Write(var value)
{
	// Implemented via hook in Engine
}

/*
 *	string GetLine(var message);
 *		Prints message and requests user input from
 *		console.
 *	Arguments:
 *		message			Message to print to console.
 *	Returns:
 *		String			User inputted string.
 */
public method GetLine(var message)
{
	// Implemented via hook in Engine
}

/*
 *	Key WaitKey();
 *		Waits for a key to be pressed in the console.
 *	Arguments:
 *		None
 *	Returns:
 *		Key				Key pressed.
 */
public method WaitKey()
{
	// Implemented via hook in Engine
}
