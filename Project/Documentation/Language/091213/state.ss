/*
 *	state.spl
 *	Revision September 12th, 2013
 *
 *	This document outlines the class subtype "state".
 *	This is one of the special embedded subtypes, the
 *	other being an "entity" which is outlined in detail
 *	in the "entity.spl" document.
 *	Swift uses a state stack model for handling collections
 *	of entities. This allows for incredible control over
 *	the game's execution state by simply modifying the
 *	state stack's state.
 *	Note that a state is NOT the same as a level, but
 *	a state can contain a level (room).
 *	States, like entities, have special properties and
 *	methods which are linked into the Engine are managed
 *	accordingly.
 */

// Subtype "state"
state TitleScreen;

/*
 *	Special Properties.
 *	Like entities, states also have special properties.
 *	These are outlined here.
 *
 *	Special Properties:
 *		RW	Camera	Camera			Camera object.
 *		RW	list	Entities		List of entities in the world.
 *		RW	TileMap	TileMap			World's tilemap.
 *		RW	Color	BackgroundColor	Background clear color.
 *		RW	int		Active			Should this state be updated?
 *		RW	int		Visible			Should this state be drawn?
 */

// Set BackgroundColor
BackgroundColor = Color.CornflowerBlue;

/*
 *	Note on constructors.
 *	When the constructor is called no special properties
 *	can be safely mutated.
 *
 *	NOTE THAT State constructors may be removed in
 *	future revisions, given the nature of entities this
 *	may actually become more of a problem for new
 *	developers than useful.
 *
 *	As a rule of thumb, just use Enter() for everything.
 */
function()
{

}

/*
 *	Special Methods.
 *	Again, like entities, states also have special methods.
 *	They follow the same rules as described for special methods
 *	in "entity.spl".
 */

/*
 *	Special Method "Enter".
 *	Called immediately after the state is added to the
 *	state stack.
 */
protected method Enter()
{

}

/*
 *	Special Method "Leave".
 *	Called immediately before the state is removed
 *	from the stack.
 */
protected method Leave()
{

}

/*
 *	Special Method "Update".
 *	Called once every frame before Draw().
 *	Game logic should be handled here.
 */
protected method Update()
{

}

/*
 *	Special Method "Draw".
 *	Called once every frame after Update().
 *	Everything the state draws should be put here.
 *	This includes backgrounds, tilemap, overlays.
 *	To draw entities a call to DrawEntities()
 *	can be made.
 *	This allows for states to add overlays and such.
 */
protected method Draw()
{

}

/*
 *	Special Method "ExitRequested".
 *	Called when the user requests the game to close.
 *	If all states return true the game will exit.
 *	Keep in mind the engine can still force close the game
 *	and not honor the result of ExitRequested().
 */
protected method ExitRequested()
{
	return true;
}
