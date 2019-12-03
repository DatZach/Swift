/*
 *	global.spl
 *	Revision September 12th, 2013
 *
 *	This document outlines the class subtype "global".
 *	You can think of a "global" as a static class in
 *	C#, only public methods can be declared.
 *	They can mutate their state but cannot
 *	define an initiation state.
 *	Global subtypes cannot be initiated.
 *	So the following is illegal:
 *		var a = Settings();
 *		a.Score = 1234;
 *
 *	The following is legal:
 *		Settings.Load("settings.json");
 *		Settings.Score = 1234;
 *
 *	These are for things like global variables.
 *	You can think of "global" as being on par with
 *	Ruby's modules.
 */

// Subtype "global"
global Settings;

/*
 *	Special case fields.
 *	Since these are all treated as static variables,
 *	and there are no methods, all variable's access
 *	modifier default to public instead of private.
 *	Private and protected accessors are illegal and
 *	will result in an error.
 */

          var Score   = 0;					// This is public
public    var Weather = WeatherFlag.Sunny;	// This is public
private   var Season  = 1;					// This is illegal
protected var FooBar  = 2;					// This is illegal

/*
 *	Special case enumerations.
 *	Follow the same rule exceptions described above.
 */

public enum WeatherFlag
{
	Sunny,
	Cloudy,
	Stormy
}

/*
 *	Constructors.
 *	Constructors cannot be declared.
 */

/*
 *	Special case methods.
 *	All methods are treated as public static.
 */
public method Load(var filename)
{
	var file = File(filename, File.Mode.Read | File.Mode.Binary);
	if (file == nil)
		return false;
	
	Score		= file.ReadDword();
	Weather		= file.ReadByte();
	Season		= file.ReadByte();
	FooBar		= file.ReadWord();
	file.Close();
	
	return true;
}

/*
 *	Methods without an explicitly
 *	stated public access modifier
 *	are still public static.
 */
method StillPublicStatic()
{

}
