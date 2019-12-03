/* 
 *	entity.spl
 *	Revision September 12th, 2013
 *
 *	This document outlines the class subtype "entity".
 *	This is one of the special embedded subtypes, the
 *	only other special embedded subtype would be "state".
 *	States are outlined in the "state.spl" document.
 *	The majority of changes revolve around a few new
 *	special methods and properties. Other than that
 *	normal class rules and syntax apply.
 */

// Subtype "entity"
entity Player
{
	/*
	 *	Optional Initializer List
	 *		Denoted by { } block after type ident header.
	 *	Only properties specific to subtype can be initialized here.
	 *	Scene Editor reads this to figure out how to draw entities.
	 *	Only fields and properties may be initialized here. No code.
	 */
	
	Sprite = Sprite("Player"),			// Using commas in the list instead of ;
	X = Parent.Size.X / 2.0,
	Y = Parent.Size.Y / 2.0,
	Z = -10
};

/*
 *	Special Properties.
 *	Entities by default have a few special properties.
 *	These are properties that the Engine will automatically
 *	keep track of and update various attributes in
 *	accordance.
 *
 *	Special Properties:
 *		R 	State		Parent		State this entity exists inside of.
 *		RW	Texture		Sprite		Texture object set from
 *									"Assets.Texture[...]".
 *		RW	Vector3f	Position	Position of entity in the game
 *									world. X, Y describe entity
 *									position in the world.
 *									Z describes entity depth.
 *		RW	Vector2f	Origin		Origin of entity.
 *		R	Vector2f	Size		Mirrors Sprite.Size.
 *		RW	Vector2f	Scale		Mirrors Sprite.Scale.
 *		R	FloatRect	BoundingBox	Automatically updated to reflect
 *									the bounding box according to
 *									the sprite or Size properties.
 */

/*
 *	Note on constructors.
 *	When the constructor is called the special property
 *	"Parent" is not yet valid.
 *
 *	NOTE THAT Entity constructors may be removed in
 *	future revisions, given the nature of entities this
 *	may actually become more of a problem for new
 *	developers than useful.
 *
 *	As a rule of thumb, just use Create() for everything.
 */
method()
{

}

/*
 *	Special Methods.
 *	These will be called by their "invisible" virtual
 *	methods if nothing is set. That meaning nothing
 *	will be executed when these are called but not
 *	defined.
 *
 *	All special methods are protected, if no access
 *	modifier is specified they will be set to
 *	protected by default. If they are defined as
 *	"private" or "public" the compiler will complain.
 */

/*
 *	Special Method "Create".
 *	Called immediately after the entity is added to
 *	the parent state. Most creation logic should
 *	be handled here INSTEAD OF the constructor.
 */
protected method Create()
{

}

/*
 *	Special Method "Destroy".
 *	Called immediately before an entity is removed
 *	from the parent state. Parent is still valid
 *	at this point in time.
 *
 *	Note that this is not a deconstructor, it works
 *	more reliably than a deconstructor would because
 *	it's called immediately upon entity removal.
 */
protected method Destroy()
{

}

/*
 *	Special Method "Update".
 *	Called once every frame before Draw().
 *	All entity logic should be handled here.
 */
protected method Update()
{

}

/*
 *	Special Method "Draw".
 *	Called once every frame after Update().
 *	All draw logic should be handled here.
 */
protected method Draw()
{

}

/*
 *	Special Method "FocusChanged".
 *	Called when the window changes focus.
 *	Arguments:
 *		focus		True if window gained focus
 *					False if window lost focus
 */
protected method FocusChanged(bool focus)
{

}

/*
 *	Special Method "KeyPress".
 *	Called when a keyboard key is pressed.
 *	Arguments:
 *		int			Key Code
 *					See Key enum
 */
protected method KeyPressed(int keyCode)
{

}

/*
 *	Special Method "KeyReleased".
 *	Called when a keyboard key is released.
 *	Arguments:
 *		int			Key Code
 *					See Key enum
 */
protected method KeyReleased(int keyCode)
{

}

/*
 *	Special Method "MouseButtonPressed".
 *	Called when a mouse button is pressed.
 *	Arguments:
 *		int			Mouse Button
 *					See MouseButton enum
 */
protected method MouseButtonPressed(int mouseButton)
{

}

/*
 *	Special Method "MouseButtonReleased".
 *	Called when a mouse button is released.
 *	Arguments:
 *		int			Mouse Button
 *					See MouseButton enum
 */
protected method MouseButtonReleased(int mouseButton)
{

}

/*
 *	Special Method "MousePosition".
 *	Called when the mouse position changes.
 *	Arguments:
 *		int			x
 *		int			y
 */
protected method MousePosition(int x, int y)
{

}

/*
 *	Special Method "MouseScroll".
 *	Called when the scroll wheel moves.
 *	Arguments:
 *		int			y
 *					Delta = 1, -1
 */
protected method MouseScroll(int y)
{

}
