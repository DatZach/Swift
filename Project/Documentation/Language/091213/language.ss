/*
 *	language.ss
 *	Revision December 16th, 2013
 *
 *	This document outlines the primary language syntax
 *	of the Swift Programming Language (SPL), working name.
 *	The language's syntax and feature set is borrowed from
 *	C#, Lua, Python, Haskell and Game Maker Language (GML).
 *	The aim is to provide an embedded scripting language
 *	for the Swift Engine. While one secondary goal would
 *	be to allow for a REPL console to let developers quickly
 *	test various methods, this might not be possible given
 *	the embedded nature of the language.
 *	The embedded nature of the language is borrowed from GML
 *	because of a single benefit, that being the ability for
 *	rapid prototyping and development due to the language
 *	being tailored to the engine.
 *
 *	Note that because this is the first version of the
 *	language that it WILL be expanded upon later.
 *	I plan on adding lambdas, delegates, events,
 *	default values, etc.
 *	These will come in time.
 *
 *	WORK ON:
 *		Dictionaries
 *		Operator overloading
 *		Method overloading
 *		Standard Library
 */

/*
 *	Standard Library.
 *	The standard library is automatically made visible in the
 *	global scope to all methods and fields in every subtype.
 *	All classes and globals are described in their respective
 *	documents.
 */

/*
 *	Reserved symbols.
 *	There are a few symbols which are reserved by the compiler.
 *	Attempting to declare a variable, enum, class or method
 *	with these names will result in an error.
 *
 *	These include:
 *		Standard Library classes
 *		Keywords
 *		"Textures"
 *		"Sounds"
 *		"Fonts"
 *		"Scenes"
 */

/*
 *	Special symbols.
 *	There are a few symbols which have special uses.
 *	The rules regarding these are outlined here.
 *	Symbols:
 *		_			Cannot be defined explicitly in a var.
 *					It's used to tell the compiler that something
 *					is not used, a = _ + 1; would be illegal because
 *					_ is "invisible".
 *					Example:
 *						foreach(var _, i in [ 1 .. 10 ]) { ... }
 *						_ is usually item, but we ignore it in this
 *						example.
 */

/*
 *	Class Subtypes.
 *	This line is basically like "PROGRAM <ident>" in PASCAL.
 *	It tells the compiler how to deal with this file and that it
 *	is a valid source file.
 *	Valid values are:
 *		class, entity, state and global
 *
 *	Technically they're all classes but the compiler treats
 *	these slightly differently by changing the visibility of
 *	certian symbols.
 *
 *	This method DOES limit classes to a single file,
 *	this might be fixed in the future. Right now it's by
 *	design.
 */
class HelloWorld;

/*
 *	Scope.
 *	Scope will be covered in greater detail in class.spl,
 *	however, a basic overview of how scopes are treated
 *	is outlined here.
 *	To make life easier the compiler will do multiple
 *	passes.
 *	Those are:
 *		1. Prototype all class subtypes.
 *		2. Prototype all fields in all subtypes,
 *		   including constants and enums,
 *		   which are resolved at this time.
 *		3. Prototype alll methods in all subtypes.
 *		4. Compile all methods and subtypes.
 *
 *	This allows for code that references fields and
 *	other subtypes to do so without having to make
 *	sure that it's declared before it's used.
 *	This makes it easier on newer programmers and
 *	speeds up the time it takes to develop a program.
 */

/*
 *	Enumerations.
 *	Enumerations are represented as they are in C#.
 *	They also take on an access modifier as described
 *	below.
 *	Note that enumerations can take on the variant
 *	type of integer. Any other attempt to assign
 *	another type will result in an error.
 *
 *	Referencing an enum value:
 *		Enum.X
 */
public enum MyEnum
{
	X,				// X = 0
	Y = 3,			// Y = 3
	Z				// Z = 4
}

/*
 *	Variables.
 *	All variables are of a variant type. The type system is
 *	simillar to that of Python or Lua. It will implicitly
 *	take on the type of whatever is assigned to it. Variables
 *	will also mutate from one type to another if assigned
 *	a conflicting type.
 *	Types:
 *		Nil			No type assigned
 *		Integer		A 64bit signed integer
 *		Double		A double precision floating point
 *		String		String of UTF-8 characters
 *		List		Dynamic list of variants, elements
 *					are stand-alone variants. Meaning
 *					the entire list does not need to
 *					contain the same variant type.
 *		Object		Abstracted pointer to object
 *
 *	Variables will implicitly take on a nil type if they
 *	are not assigned a value upon declaration.
 *
 *	Booleans are not a datatype, any time a boolean is used
 *	it will be returned as int(1) or int(0).
 *	The built-in constants "true" and "false" will be
 *	defined as int(1) and int(0), respectively.
 *
 *	In the field sample code below what declares these types
 *	is outlined.
 */

/*
 *	Typecasting.
 *	Typecasting is fairly simple, again borrowed from Python.
 *	The purpose of using the form type(x) instead of (type)x,
 *	is that the former allows for better scoping.
 *	Example:
 *		var a = int(2.0 + 3.0);
 *		vs.
 *		var a = (int)(2.0 + 3.0);
 *
 *	If an illegal casting operation is performed the type will
 *	become of type "nil".
 *
 *	Typecasting is treated as an inline function to an extent.
 *	To reference a typecast use their ordinal names.
 *	Type Ordinals:
 *		nil			"nil"
 *		integer		"int"
 *		double		"double"
 *		string		"str"
 *		list		"list"
 *		dictionary	"dict"
 *		object		"object"
 *
 *	Note on casting types to strings.
 *	Every type can be casted to a string with various different
 *	effects.
 *	Effects:
 *		str(nil)		"nil"
 *		str(1)			"1"
 *		str(2.0)		"2.0"
 *		str("hello")	"hello"
 *		str([ 1, 2 ])	"[ 1, 2 ]"
 *		str({ "a": 2 })	"{ "a": 2 }"
 *		str(object)		This depends on if method str()
 *						is overridden in the object.
 */

/*
 *	Implicit types.
 *	This section is primarily going to outline different ways
 *	literals can be defined and recognised by the compiler,
 *	but it will also outline what certian types get implicitly
 *	defined as by the compiler.
 *
 *	Integers:
 *		1				Decimal
 *		0x01			Hex
 *		0b00000000		Binary
 *	Doubles:
 *		1.0				Decimal
 *	Strings:
 *		"String"		String
 *		@"String\"		Raw string
 *	Lists:
 *		[ 1, 2, "C" ]	List
 *	Dictionaries:
 *		{ "a": 1 }		Dictionaries
 *	Object:
 *		MyClass()		Object
 */

/*
 *	Operator Precedence.
 *	The precedence table is simillar to that of C++'s
 *	or C#'s, however closer to C#.
 *	
 *	Primary			x
 *					x.y
 *					f(x)
 *					a[x]
 *					x++
 *					x--
 *					Object()
 *					typeof(x)
 *	Unary			+
 *					-
 *					!
 *					~
 *					++x
 *					--x
 *					typecast(x)
 *	Multiplicative	*
 *					/
 *					%
 *	Additive		+
 *					-
 *	Shift			<<
 *					>>
 *	Relational		<
 *					>
 *					<=
 *					>=
 *	Equality		==
 *					!=
 *	Logical AND		&
 *	Logical XOR		^
 *	Logical OR		|
 *	Conditional AND	&&
 *	Conditional OR	||
 *	Ternary			?:
 *	Assignment		=
 *					*=
 *					/=
 *					%=
 *					+=
 *					-=
 *					<<=
 *					>>=
 *					&=
 *					^=
 *					|=
 */

/*
 *	Fields.
 *	A field is a variable with special scoping rules.
 *	All fields have a access modifier in a class subtype.
 *	This works just like C#'s access modifiers.
 *	Access Modifiers:
 *		public, private, protected
 *
 *	Any field declared without an explicit modifier will
 *	be implicitly declared as private.
 */

public    var MyFieldNil	= nil;					// Declared as a nil
public    var MyFieldInt    = 1;					// Declared as an integer
private	  var MyFieldDouble	= 1.0;					// Declared as a double
protected var MyFieldString = "Hello, World!";		// Declared as a string
          var MyFieldArray  = [ 1, 2.0, "A" ];		// Declared as an list, implictly private
public    var MyFieldDict   = { "a": 1, "b": 2 };	// Declared as a dictionary
public    var MyFieldObject = HelloWorld();			// Declared as an object
public static var MyStaticField = 1234;

/*
 *	Constants.
 *	All constants are treated like static fields.
 *	This means access modifier rules and assignment
 *	rules carry over.
 */

public const MyConstInt = 1024;			// Declared a constant 1024

/*
 *	Methods.
 *	All methods are identified with a "method" keyboard.
 *	This is because all variables are variant, this allows
 *	for type ambiguity.
 */

/*
 *	Method Access Modifiers.
 *	These work exactly like field access modifiers as
 *	described above.
 */

/*
 *	Constructors.
 *	Constructors are declared as normal methods without
 *	an ident, this solves the problem of subtype name changes
 *	requiring you to refactor the name of the constructor,
 *	like in C++ and C#.
 */
public method()
{
	// Any method that reaches the end of execution
	// without returning will return a nil var.
}

/*
 *	Deconstructors.
 *	Deconstructors do not exist in this language because
 *	of the usage of a Garbage Collector (GC). This makes
 *	using deconstructors unpredictable because objects
 *	aren't deconstructed when you leave a scope but
 *	whenever the reference count reaches 0 and the
 *	GC decides to sweep unreferenced objects.
 */

/*
 *	Argument declaration.
 *	This is done just like any C like language.
 *	Because variables are all of variant type we
 *	expect the "var" keyword to be before any ident.
 *	This may be redundent and may be changed later.
 *	These arguments are treated like local variables,
 *	how those are treated is described in the section
 *	"Local Variables" below.
 */
public method FooBar(var a, var b)
{
	// Field accessed without a this or base keyword
	return MyFieldDouble * b + a;
}

/*
 *	Operations.
 *	The following annotated method will outline
 *	all basic operations supported by the
 *	language.
 */
public method Operations()
{
	/*
	 *	Local Variables.
	 *	These cannot be accessed outside of the scope of
	 *	the current method. They're allocated on the stack
	 *	and will be automatically freed and dealt with by
	 *	the GC upon leaving the scope of the method.
	 *	All rules regarding variables still apply to local
	 *	variables.
	 */
	var a = 1, b = 2, c = 4;
	
	/*
	 *	Variable assignments can also be expressions.
	 *	Also note that proper operator precedence
	 *	was followed.
	 */
	var d = a * 3 + c;				// Simplifies to d = 7;
	
	// Examples of operator precedence
	var e = GameSettings.A * -d << 2;
	var f = ++FooBar(2.0, 3.0);
	var g = c | b & a;
	var h = e < f == true;
	g <<= 4;
	
	// Some string operations
	var l = "Hi " * 4;				// "Hi Hi Hi Hi "
	
	/*
	 *	Working with lists.
	 *	Lists are a built-in type to the compiler and
	 *	these following methods and properties will
	 *	auto-resolve at compile time.
	 *	Also note that built-in type methods and properties
	 *	are always lowercase instead of following the
	 *	traditional PascalCasing.
	 *
	 *	Properties:
	 *		size				Number of elements in a list.
	 *
	 *	Methods:
	 *		[index]				Returns element at index.
	 *							Returns nil if out of bounds.
	 *		head				Return first element in a list.
	 *		tail				Return all elements except first in a list.
	 *		last				Return last element in a list.
	 *		init				Return all elements except last in a list.
	 *		push(e)				Append a new element to the end of the list.
	 *		pushrange(e)		Append a list to this list as individual elements
	 *		pop					Remove an element at the end of a list.
	 *		poprange(c)			Remove count number of elements from the end of the list.
	 *		insert(i, e)		Insert a new element somewhere in the list.
	 *		insertrange(i, e)	Insert a list into the list as individual elements at an index.
	 *		remove(e)			Remove an element from somewhere in the list.
	 *		reverse				Returns a list with the elements reversed.
	 *		index(e)			Returns the index of an element starting at
	 *							some position.
	 *		empty				Returns if a list has any elements.
	 *		contains(i)			Returns if list contains an element
	 *
	 *	You can define list ranges like so:
	 *		var a = [ 1 .. 10 ]
	 *		-> [ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 ]
	 */
	
	var i = [ 1, 2, 3, 4 ];		// [ 1, 2, 3, 4 ]
	i.push(5);					// [ 1, 2, 3, 4, 5 ]
	i.insert(2, 4);				// [ 1, 2, 4, 3, 4, 5 ]
	i.pop();					// [ 1, 2, 4, 3, 4 ]
	i.remove(0)					// [ 2, 4, 3, 4 ]
	i.head()					// 2
	i.tail()					// [ 4, 3, 4 ]
	i.reverse()					// [ 4, 3, 4, 2 ]
	i.head()					// 4
	i.tail()					// [ 3, 4, 2 ]
	
	/*
	 *	2D lists
	 *	One way to do 2d lists is [ [], [] ].
	 *	This is obviously not optimial for a lot of reasons.
	 *	Because of this a stdlib class Grid is provided.
	 *
	 *	var grid2d = Grid(x, y);
	 *	var grid3d = Grid(x, y, z);
	 */
	
	/*
	 *	Working with dictionaries.
	 *	Dictionaries are a built-in type that work like a map.
	 *	Implementation is simillar to that of Python or a
	 *	built-in Dictionary type in C#.
	 *	These have special properties and methods.
	 *
	 *	Dictionaries can have any type variant excluding
	 *	objects, dictionaries and nil as the key,
	 *	any type can be a value.
	 *
	 *	Properties:
	 *		size				Number of elements
	 *
	 *	Methods:
	 *		[key]				Return value that matches key.
	 *							Returns nil if key doesn't exist.
	 *		add(k, v)			Set key to value.
	 *							Returns false if key exists.
	 *		remove(k)			Remove key.
	 *							Returns false if key doesn't exist.
	 *		exists(k)			Returns if key exists.
	 *		keys				Returns list of keys.
	 *		values				Returns list of values.
	 *		list				Returns list of key-value pairs.
	 *							[ [ "a", 1 ], [ "b", 2 ] ]
	 */
	var o =
	{
		"a": 1,
		"b": 2,
		3: "c",
		4.2: HelloWorld()
	};
	
	o.size();					// 4
	o.add(1.2, o["a"])			// o[1.2] = 1
	
	/*
	 *	Working with objects.
	 *	Objects are allocated at runtime and are managed
	 *	by the Garbage Collector via reference counting.
	 *	Whenever the reference count becomes 0 the GC will
	 *	free the object from memory.
	 *	The following section outlines various operations
	 *	regarding objects
	 */
	
	/*
	 *	This will allocate a new object, there is no
	 *	"new" operator. Like Python objects are created
	 *	by calling them like a function. This also helps
	 *	reinforce the idea that our constructors are just
	 *	nameless functions that take on their subtype's
	 *	ident.
	 */
	var j = HelloWorld();
	
	// Call public method FooBar()
	var k = j.FooBar(2.0, 3.0);
	
	// Mutate public field MyFieldInt
	++j.MyFieldInt;
	
	/*
	 *	String operations.
	 *	This section will outline all string operations.
	 *	These will work like the list operations in terms
	 *	of implementation.
	 *
	 *	Raw strings (nothing is escaped) are represented
	 *	with "@" before the string literal.
	 *
	 *	Escaping Characters:
	 *		\'					', this charactercan be in
	 *							a string without being escaped.
	 *		\"					"
	 *		\\					\
	 *		\%					%
	 *		\:					:
	 *		\0					null
	 *		\a					Alert
	 *		\b					Backspace
	 *		\f					Form feed
	 *		\n					New line (All strings wrap with \n)
	 *		\r					Carriage return
	 *		\t					Horizontal tab
	 *		\u					Unicode escape sequence (hex)
	 *							\u0041 = "A"
	 *		\U					Same as \u
	 *		\v					Vertical tab
	 *		\x					Raw byte translated into character
	 *							\x0041 = "A"
	 *							\x7F = chr(0x7F)
	 *
	 *	Multiline strings:
	 *		var a = "Hello, "
	 *				"World!";
	 *
	 *	Note that multiline strings do not automatically add new lines
	 *	in the string literal.
	 *
	 *	Formatting:
	 *		String formatting is designed as a mix between C# and
	 *		Python. I think the fusion results in a very clean
	 *		and easy to type and read syntax.
	 *
	 *		var a = "%0, %1. 0x%2:x is a good number." % [ "Hello", "World", 0x1337 ];
	 *			"Hello, World. 0x1337 is a good number."
	 *		var b = "%0\: Rapid game development made easy." % [ "Swift" ];
	 *			"Swift: Rapid game development made easy."
	 *		var c = "I'm %0\% sure that %0\% is a good success rate." % [ 100 ];
	 *			"I'm 100% sure that 100% is a good success rate."
	 *		var d = "An example of a list would be, %0." % [ 1, 2, 3, "take", "on", "me" ];
	 *			"An example of a list would be, 1 2 3 take on me."
	 *		var e = "%0 is an object." % [ HelloWorld() ];
	 *			"{object} is an object."
	 *			{object} will be formatted unless the object has an
	 *			overridden str() method.
	 *
	 *	Operators:
	 *		+					Concat, assign, unary
	 *		*					Multiply, str *= int only, assign, unary
	 *		%					Format, unary
	 *		[]					Indexer, unary
	 *
	 *	Properties:
	 *		length				Number of characters
	 *		size				Size of string in bytes
	 *
	 *	Methods:
	 *		[index]				Returns the character at index.
	 *							Returns nil if out of bounds.
	 *		upper				Returns an uppercase string.
	 *		lower				Returns a lowercase string.
	 *		swapcase			Returns a string with casing swapped.
	 *		substring(i, c)		Returns substring starting at index and of count characters.
	 *		insert(i, s)		Returns string with string inserted at index.
	 *		overwrite(i, s)		Returns string with inserted at index, overwriting contents.
	 *		replace(o, n)		Return string with old substring replaced with new substring.
	 *		first(s, i)			Returns first index of substring starting at index.
	 *							Returns -1 if nothing is found.
	 *		last(s, i)			Returns last index of substring starting at index.
	 *							Returns -1 if nothing is found.
	 *		strip(s, c)			Returns string with count occurrences of substring removed.
	 *							A 0 count would remove all occurrences.
	 *		replace(o, n, c)	Returns string with count occurrences of old substring
	 *							replaced with new substring.
	 *							A 0 count would replace all occurrences.
	 *		startswith(s)		Returns if a string starts with substring.
	 *		endswith(s)			Returns if a string ends with substring.
	 *		isalphanum			Returns if a string is entirely alphanumeric.
	 *		isalpha				Returns if a string is entirely alpha.
	 *		isnum				Returns if a string is entirely digits.
	 *		islower				Returns if a string is entirely lowercase.
	 *		isupper				Returns if a string is entirely uppercase.
	 *		splitlines			Returns a list of lines broken into elements.
	 *							A line denoted by \n, not \r\n.
	 *		contains(s)			Returns if a string contains subtring.
	 *		padleft(s, c)		Return string padded left count times with substring.
	 *		padright(s, c)		Returns string padded right count times with substring.
	 *		split(d)			Returns list split into a list using delimiter.
	 *							Delimiter character is removed and is not included
	 *							in list string elements.
	 *		trim				Returns string with leading and trailing whitespace removed.
	 *		list				Returns a list of every character in an element.
	 *							This is to allow for iteration over every character in an
	 *							elegant manner.
	 *							foreach(var c in "Hello, World!".list()) { ... }
	 */
	
	var m = "Hello, World!";
	var n = m[0] * 3 + m.substr(1, -1);		// "HHHello, World!"
	
	/* I should probably add more examples, but I'm too lazy. */
}

/*
 *	Built-in functions.
 *	These are special case functions that have various
 *	uses. These do not make calls to the engine, but
 *	instead resolve to either an inline method or
 *	some special opcode.
 */
private method BuiltInFunctions()
{
	/*
	 *	typeof(x)
	 *	Returns the type of a variable.
	 *	Returns these as a string that represents
	 *	their ordinal names.
	 *	Ordinal names are descibed in the section
	 *	"Typecasting".
	 */
	var a = 1;
	var b = 2.0;
	var c = [ a, b ];
	
	typeof(a);					// int
	typeof(b);					// double
	typeof(c);					// list
}

/*
 *	Control statements.
 *	These are the program flow statements and
 *	their rules. These are borrowed from C#,
 *	Python and GML.
 */
private method ControlStatements()
{
	/*
	 *	Note on relational truths.
	 *	All implicit relations are required to be
	 *	a type of int or double.
	 *	Valid:
	 *		if (3)
	 *		if (true)
	 *		if (5.3)
	 *		if ([ 1, 2, 3 ].contains(1))
	 *	Invalid:
	 *		if ("Hello?")
	 *		if ([ 1, 2, 3 ])
	 *		if (Object())
	 */
	
	/*
	 *	If Statement.
	 *	This is a C style if statement.
	 *	Implicitly compares relations to true.
	 *	if (true) would branch.
	 */
	if (Assets.Config.Fullscreen == true)
	{
		// Some code
	}
	/*
	 *	Else Statement.
	 *	This is a C style else statement.
	 *	Used in junction with an if statement.
	 *	else if (...) { } valid.
	 */
	else
	{
	
	}
	
	/*
	 *	Switch Statement.
	 *	This is a C# style switch statement.
	 *	You can switch to any type except lists
	 *	and objects.
	 */
	switch(Assets.Config.TextureSheetSize)
	{
		// Case Statement
		case 2048:
			/*
			 *	Break Statement.
			 *	Break statements can take an operand which
			 *	will specify how many scopes to break from.
			 *	By default it will break by 1 scope.
			 *	Example:
			 *	while(true)
			 *		break;			// Break from while(true)
			 *						// Alternate syntax: break 1;
			 *
			 *	while(true)
			 *	{
			 *		while(true)
			 *			break 2;	// Break from while(true) { while(true) { ... } }
			 *	}
			 */
			break;
		
		case 4096:
			break;
		
		// String type case
		case "foobar":
			break;
		
		/*
		 *	Default Case.
		 *	Called if the switch cannot meet any case.
		 *	Breaks on default are required.
		 */
		default:
			break;
	}
	
	/*
	 *	Do-While Statement.
	 *	This is a C style do-while statement.
	 */
	do
	{
	
	}
	while(true);
	
	/*
	 *	While Statement.
	 *	This is a C style while statement.
	 */
	while(Assets.Config.Fullscreen)
	{
	
	}
	
	/*
	 *	For Statement.
	 *	This is a C style for statement.
	 *	Note on the comma operator, the comma operator
	 *	is ONLY valid in the scope of the for() and var
	 *	declarations.
	 *	Valid:
	 *		for(var i = 0, j = 1; i < j; ++i, --j)
	 *			;
	 *
	 *	Invalid:
	 *		var a = 0, b = 1;			// Legal
	 *		++a, --b;					// Illegal
	 */
	for(var i = 0; i < 10; ++i)
	{
	
	}
	
	/*
	 *	Foreach Statement.
	 *	This is (mostly) a C# style foreach statement.
	 *	Note that the "in" keyword is ONLY valid in
	 *	the scope of foreach().
	 *	Note on enumerations, foreach will only work
	 *	on a primitive variant list, if you would like
	 *	to iterate over a list of objects add them to
	 *	a primitive list, like so:
	 *		foreach(var x in [ Object("Foo"), Object("Bar"), Object("Baz"))
	 *		{
	 *			...
	 *		}
	 *
	 *	Iteration over objects may be added in the future.
	 *	Possibly going the Python route and providing
	 *	a special method to return an iterator?
	 *
	 *	Note on special syntax:
	 *		foreach(var <item>, <index> in <list>) { ... }
	 *		foreach(var <item> in <list>) { ... }
	 *	These are BOTH valid, the former will also set <index>
	 *	to be the current element's index, and <item> to be the
	 *	element of the index.
	 *	Inspired by Python, obviously.
	 */
	foreach(var a, i in [ 1, 2, 3, 4 ])
	{
		// Skip the second item for instance
		if (i == 1)
		{
			/*
			 *	Continue Statement.
			 *	This is a C style continue statement.
			 *	Special "break" syntax does not apply here.
			 */
			continue;
		}
	}
	
	/*
	 *	Repeat Statement.
	 *	This is a GML style repeat statement.
	 *	Really, it's just short handle for:
	 *		for(var i = 0; i < x; ++i) { ... }
	 */
	repeat(16)
	{
	
	}
	
	/*
	 *	Return Statement.
	 *	This is a C style return statement.
	 *	Note that "return;" will actually return a variant.
	 *	That variant will always be of type "nil" though.
	 *	Methods that reach the end of the scope and do
	 *	not return will return "nil" by default.
	 */
	return 0;
}

/*
 *	Error handling.
 *	SPL does not have exceptions for many reasons.
 *	They're ugly, dangerous, hard to work with
 *	cleanly and hard to implement.
 *	Without exceptions you're limited to returning
 *	error codes when something goes wrong. However,
 *	this is not the prefered method of dealing with
 *	errors. Erroneous operations should return nil.
 *	The calling method should then check if the type
 *	is nil and then handle the error appropriately.
 *
 *	THIS IS NOT FINAL.
 *	I'm aware that this is weaker than both error
 *	codes and Exception's metadata. Maybe a variant
 *	type of "error" that provides string metadata?
 */
private method UnsafeDivide(var a)
{
	// Prevent divide by zero errors
	if (a == 0)
		return nil;
	
	return 100.0 / a;
}

private method ErrorHandling()
{
	var a = UnsafeDivide(0);
	if (a is nil)
		return 0.0;
	
	return a * 10.0;
}

public static method StaticMethod()
{
	// Static method
}

/*
 *	Special methods.
 *	There are a couple special methods that will cause
 *	the compiler to treat some instances slightly differently.
 *	These change depending on the subtype, but these are
 *	universal to all class subtypes.
 *
 *	Note that all special methods MUST be public and that
 *	these names are reserved, any private or protected
 *	methods with these names will result in a compile-time
 *	error.
 */

/*
 *	method str()
 *	Return a string representation of the object.
 */
public method str()
{
	return "HelloWorld " + str(MyFieldInt);
}

/*
 *	method call()
 *	Executed when an object is referenced like so:
 *	var f = Object();
 *	f();
 *
 *	Note that variables can be passed through this.
 */
public method call(var x, var y)
{
	return FooBar(x, y);
}

/*
 *	Preprocessor Directives.
 *	These are very special directives, there are not
 *	a lot of these because I think they're ugly.
 *	However, they are very useful in some situations,
 *	especially since this language is designed to be
 *	cross-platform, there needs to be a way to allow
 *	developers to specify platform specific code sometimes.
 *
 *	Directives:
 *		#if					If
 *		#endif				End If DEFINED
 *		#else				ELSE
 *
 *	Definitions:
 *		TARGET_PLATFORM		This will be a number that
 *							specifies what platform we're
 *							currently targeting.
 *		PLATFORM_WINDOWS	Windows (0)
 *		PLATFORM_MACOSX		Mac OS X (1)
 *		PLATFORM_LINUX		Linux (2)
 *		PLATFORM_IOS		iOS (3)
 *		PLATFORM_ANDROID	Android (4)
 *		PLATFORM_HTML5		HTML5 (5)
 */

#if TARGET_PLATFORM == PLATFORM_WINDOWS
public PlatformSpecific()
{

}
#else if TARGET_PLATFORM == PLATFORM_MACOSX
public PlatformSpecific()
{

}
#endif
