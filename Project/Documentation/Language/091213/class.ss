/*
 *	class.spl
 *	Revision September 12th, 2013
 *
 *	This document outlines the class subtype "class".
 *	These are normal vanilla classes, which were mostly
 *	described in the document "language.spl".
 *
 *	There's a few features that will be described here
 *	in future revisions of the language, which is why
 *	this document exists.
 *	Among these is polymorphism.
 */

// Subtype "class"
class MyClass;

/*
 *	Structures.
 *	There is no normal "structure" subtype.
 *	This is because classes and structures are
 *	the same thing, except one has methods and
 *	the other does. This felt too redundent to
 *	provide a special case subtype.
 *	All structures should just be a class with
 *	all public fields.
 */

/*
 *	Scoping.
 *	Scoping is done exactly like C#, respecting
 *	features that exist in SPL. Since there
 *	are no static methods or first-class methods
 *	you cannot reference a function through the
 *	subtype's name, only through the instance.
 *	Levels of scoping:
 *		Block			method f(var x)
 *						{
 *							var a = 2;		// "x" and "a" are
 *							return a * x;	// in block scope.
 *						}
 *		Module			public var X;		// "X" is in module scope.
 *
 *	Valid references from non-initiated classes:
 *		Class.Enum.Value
 *		Global.Field
 *		Global.Method
 *
 *	Invalid references from non-initiated classes:
 *		Class.Method
 *		Class.Field
 *
 *	Valid references from initiated objects:
 *		Object.Field
 *		Object.Method
 *
 *	Invalid references from initiated objects:
 *		Object.Enum.Value
 */

// Field
public var A = 0;

/*
 *	Properties
 *	These are implemented much like C#'s properties.
 */

public var Name { get; private set; }

// Constructor
method(var a, var b)
{

}
