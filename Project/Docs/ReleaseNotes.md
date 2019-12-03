## Release Notes

### Update v0.13-_______

* Fixed File -> Open not filtering .sproj
* \#0000121: Type signature listed for Set() in Grid.md is incorrect
* \#0000122: Swift doesn't like nility checks
* \#0000123: CTD when deleting a script
* Fixed IDE starting with error after update
* Fixed Swift injecting its install into every folder a project is opened from.
* Fixed a crash regarding event dispatching in very specific cases.

### Update v0.12-e67207e

* Modified project format. Old projects must have their "project.json" renamed
  to "project.sproj".
* IDE can now load .sproj directly from the disk via file associations.
* Audio engine failing to initialize is no longer fatal. Sounds will not play in such an event now.
* Scene Editor rewritten using native Qt instead of SFML.
* Changes to scenes are now saved properly
* Added some extra control over automatic toggling of the shell window during various events.
* Status message for compiling and running games added.
* A bunch of improvements over the old Scene Editor made during the rewrite.
* IDE will prompt you asking if you really want to exit if a game is currently running now.
* IDE will prompt you asking if you want to reload a script off of the disk if it has changed now.
* IDE automatically syncs textures from the disk now.
* Fixed a few bugs with certian widgets in the IDE not getting focus by default.
* \#0000090: CTD when you close certain assets
* \#0000030: Can't place entities at negative posisitons
* \#0000031: Editing Scene Properties doesn't work
* \#0000036: Scene Key Bindings
* \#0000003: Clicking SFML view does not focus IDE
* \#0000011: Switching between two SFML view tabs causes flicker
* \#0000032: Deleting entities is unintuitive
* \#0000033: States and Entites can't be added
* Fixed running to error when compile fails
* \#0000089: Tabbing multiple lines of code

### Update v0.11-db0874a

* Fixed properties not being able to evaluate object reference
* Engine no longer uses 100% CPU and GPU (REVERTED BEFORE PUSH)
* Variant errors now report the method and IP it occurred in.
* Argument type errors report method and IP now
* Nested comments are highlighted correctly in IDE now
* Various other minor fixes and additions to the IDE
* Fixed a pretty serious bug in the compiler
* Garbage Collector FINALLY works!

### Update v0.10-a94329f

* Reverted to Class.MethodName/PropertyName because fuck Java
* Finally fixed the packaged examples. Uses the most up to date version of the API now.
* Fixed \" not escaping correctly
* Added len()
* Compile time classes are removed from compiled game like they should be
* Comment nesting works
* \#0000118: GIB POINTLESS CODE BLOCKS AND NESTED MULTI-LINE COMMENTS
* Math functions accept numbers instead of doubles now (number = int or double)
* \#0000119: YOUR SHITS FUCKED WITH THE DOUBLE CONVERSION OR SOME SHIT HOLY SHIT
* \#0000120: clamp function
* Various additions to the stdlib. Math.Lerp/RLerp/Clamp, Debug.Log, Random.Next(max)/(min, max), etc
* \#0000117: Field lookup fails for no apparent reason
* Switch statement compilation fixed
* Proper variable scoping added
* \#0000116: Optional type annotation for some primitives fails to compile.
* Various other smaller bugs with compiler fixed.
* "string".Method() syntax added, string methods added.
* "String %0" % [ "formatting!" ] added
* List methods added
* method call() systax added
* Added option in IDE Preferences to show disassembly post-compile.

### Update v0.9-3aed3b0

* Fixed arguments in constructor not compiling
* Fixed a fairly serious error regarding constructors being called at the wrong times
* Fixed initializer list not working 
* Fixed entity sprite not taking x/y if they were set as integers
* KeyHeld and MouseButtonHeld events added to states and entities
* Pong example added
* \#0000060: Keyboard class is fucked
* \#0000059: Mouse class is fucked
* Mouse.GetX/Y() replaced with a property Mouse.X/Y

### Update v0.8-d7bada7

* Added method overload support
* Sprite DrawDD(x, y) is now Draw(x, y)
* Color has 2 new constructors Color() and Color(r, g, b)
* Fixed a bug with the error highlighting
* Added properties!
* Fixed a nasty bug with setting fields of another class
* Standard Library classes updated to use properties
* Added support for descending indexers (a[0][1] = 5; a[0][1])
* Added "is" keyword for typechecking
* Added explicit typing via var x : Object

### Update v0.7-9fd0de6

* Added IDE preferences
* \#0000014: Save IDE customization somewhere
* Grid() initializes all elements to nil now
* Added TextFile stdlib class
* Added BinaryFile stdlib class
* Fixed syntax highlighting for hex and binary numbers
* Various modifications to the Engine regarding initialization.
* System exceptions are no longer caught at top level allowing the Engine to crash "correctly".
* Added FileSystem stdlib class
* Added Math stdlib class
* Renamed Console stdlib class to Debug. Added trace, warn and error methods.
* \#0000068: Inline decrement on non-variable causes no compiler error

### Update v0.6-25af0a2

* Added Grid class
* IDE runs to compile error if one occurred now
* \#0000056: Sprite origins don't actually work
* \#0000010: More tab functionality
* \#0000016: IDE does not validate Initial State
* Scene Editor parses initializer list to determine what texture to use now.
   (Removed the "//!entity" header from being required in entities)
* Loading new textures in texture form displays them without needing to save and reload now
* Texture Editor Form now uses a native Qt graphics view instead of an SFML view.
   (Significantly faster, no flicker, fixes some rendering problems, less memory overhead, etc)
* Fixed texture form's loading dialog to include correctly supported image types
* \#0000088: Texture viewer has the same focus problems the scene editor does
* Can define, get and set static fields. Can define static methods now.
* Can no longer create classes that collide with the stdlib.
* \#0000078: Arguments to Color() constructor don't appear to be bounded or typechecked
* Various fixes to the examples

### Update v0.5-99552e3

* Initializer lists work even if method Create() isn't declared now
* State's Enter() and Leave() have been renamed Create() and Destroy() to match entities
* Fixed asset renaming
* Asset creation dialog to replace old asset creation pipeline

### Update v0.4-d6c8c77

* \#0000109: Foreach has unpredictable behavior when used on a non-list
* \#0000107: Extended foreach: index is value and value always nil
* \#0000097: Swift doesn't do anything sane when I index a list with a list.
* \#0000099: Swift dies when stringifying a list that contains itself
* Fixed return statement not accept nil return (return;)
* \#0000101: Swift dies on augmented assignment with lists
* \#0000102: Swift doesn't really understand how augmented assignment works with lists
* \#0000110: Unhelpful error message when performing supported operation between unsupported types
* \#0000098: Swift for some reason converts indices to unsigned when performing list lookup: strange errors
* Negative indices are now supported on strings and lists (a[-1] == a[a.size()* 1])
* Compiler will now compile 'this' identifier. (GetParentState().Remove(this))
* \#0000096: Behavior for list + is inconsistent
* \#0000112: Incorrect variant error for certain operators between nils (and maybe other types)
* \#0000113: Lists ignore missing commas
* \#0000115: Actually, wait. Lists aren't getting randomly flattened: they're flat-on-arrival
* Fixed [1..4] resulting in a cryptic error.
* \#0000105: Increment operator (++) should not be defined on lists
* \#0000100: In-place increment/decrement operators don't work with lists
* Framerate statistics won't show up in engine debug output if none were collected now

### Update v0.3-ce99742

* var x; will set var to nil instead of not allocating a var at all now
* Game.Close()
* Fixed while statements (again)
* Fixed foreach statements (again)
* Optimizations in the VM and Compiler
* \#0000095: Engine produces [Trace] lol on run

### Update v0.2-edd6f8f

* Removed unnecessary reference in Compiler
* \#0000093: Text Wrapping is kind of meh
* Fix inline declaration of lists in fields
* Fix crash when accessing lists with an index of len
* Lists are now lifetime objects
* Minor version will now increment every push

### Update RevTag 761f0e0
 *THIS IS A BREAKING CHANGE*

* \#0000040: No output appears for me in the Engine Debug pane
* \#0000037: Expected "identifier" got "" instead.
* Fixed disassembly output
* \#0000064: ++ operator works halfway
* \#0000043: Really interesting behavior when variable names shadow method names
* Fixed framerate jitter in the Engine
* Framerate statistics
* prefix ++/-- operators should work on fields now
* Unary + operator (absolute value) works now
* Removed print keyword
* typeof() accepts expressions now
* Temp variables are unique, should fix some possible problems
* break implemented
* continue implemented
* Initializer lists
* Asset namespaces (classes)
* Optionally mute games when running (checkable from the Build menu)
* Added a few missing keywords to the syntax highlighter
* Script Editor no longer opens at the bottom of the script
* IDE uses native path seperators now
* Raw strings (unescaped strings) are supported as per spec
* Public fields that are only one character are now lowercase (x, y, z, r, g, b, a, etc)
* Proper error reporting
* Referencing constants should respect visibility of symbol now
* Fix enum a { x, y z } being valid syntax
* var a = x.y; x.y = c; syntax works now
* Fixed a crash when entities had no sprite assigned to them
* switch statement partially implemented
* Fixed a crash in the VM when executing typeof(null)
* Fixed while, do-while and for statements
* LISTS!!!!
* Added foreach statement
* Added .. operator
* Console.WriteLine() accepts all types
* Various other minor fixes to compiler and compiler output
* Creating new assets in the IDE automatically opens them in the workspace now

### Update RevTag 7b28dc6

* \#0000074: Unhelpful error message when declaring the same variable multiple times
* \#0000049: Creating new script should append .ss to it automatically (actually fixed in revtag 4b8ed28)
* \#0000062: || doesnt work
* \#0000019: Editing scene while configuration is open
* Refactoring of the entire IDE
 * NOTHING EXCEPT WHAT IS EXPLICITLY STATED SHOULD BE FIXED. NOTHING SHOULD BE BROKEN.
* New Project Dialog focuses project name line edit automatically now.
* Welcome Dialog's recent projects will no longer show non-existant projects
* Script Editor tab widths are now correct
* \#0000035: Double clicking already open assets does not switch tab
* Inline renaming of assets instead of modal dialog
* Confirmation dialog when deleting assets
* \#0000051: Deleting Scenes while they are opening CTD
* \#0000039: Opening an already open tab
* Removed the State and Entity folders, will be reimplemented as subfolders of Scripts later.
* \#0000025: Deleting project directories leaves them without a name in the project selection window
* Fixed bug in creating new assets where names might overlap
* New scenes are the size of the window instead of a default 320x240
* Configuration dialog is no longer modal
* \#0000065: Project Explorer context menus should provide Add *

### Update RevTag 4b8ed28

* \#0000013: IDE lies about supported texture formats
* \#0000007: Dies on 'swift help update'
* \#0000023: You can rename assets (or at least Scripts) to names that are invalid for them
* \#0000027: Compile an empty project
* \#0000041: Remove the .ss from a script* Won't compile (or include it in compile?)
* \#0000044: Nil causes a variant error when being casted
* Fixed objects (include null objects) erroring when being casted to strings
* \#0000045: Casts should be constructor calls (you jerkbag)
* \#0000034: Save All doesn't work
* \#0000052: Scenes are small or something
* \#0000066: Doesn't ask to save before closing
* \#0000084: Console class isn't colored like the rest
* \#0000087: Textures don't update
* \#0000015: Unsaved changes warning
* \#0000004: Scene editor receives input when not focussed

### Update RevTag 8417796

* Actually fix issue \#0000012
* Actually fix issue \#0000009
* \#0000082: Games will not start (error)
* \#0000086:* is still broke
* \#0000077: The compiler seems to think reserved words make cool variable names
* \#0000069: * operator not implemented for str
* \#0000071: Nonsensical output from concatenation of integer to string
* \#0000073: Variables defined nowhere are a compiler error but should be nil
* \#0000076: I don't know what Color (the variable) is but the engine doesn't like it when I convert it to a string.
* \#0000085: Sprite.DrawDD is still broken
* \#0000079: What does Swift think this *means*? (i.e. why does this compile and what's with the bytecode)
* \#0000061: Can't do maths in str()
* \#0000081: if() should check (!=false), not (==true)
* Fix comparing doubles and integers with ==, !=, <, >, <= and >=
* Scenes clear when loading a new one

### Update RevTag 0d06ad6

* \#0000006: Numbers have same color as comments in editor
* Improve script editor's looks a little
* No longer need to be registered to create a new project
* \#0000021: Trying to create project in non-existant directory
* \#0000059: Mouse class is fucked
 * Mouse.GetX/Y() shouldn't take arguments
* \#0000075: Something very strange is going on here with regard to argument order
* \#0000057: -='ing stuff just makes it -360+difference or something ITS BROKE
* \#0000012: Instance fails to run immediately after starting.
* \#0000009: Sound doesn't work
* \#0000053: Can't load a scene without setting ClearColor beforehand
* \#0000050: Update GLFW