/*
 *	Primary.ss
 *	Primary game state
 *	IMPORTANT NOTES!
 *		YOU MUST CREATE AT LEAST ONE OF THESE
 *		YOU MUST SPECIFY AN INITIAL STATE IN "Configuration"
 *
 *	NOTES
 *		Semicolons are optional
 *		"state Primary" is just as valid as "state Primary;"
 */

// Declare this as a state
state Primary;

// Declare a static field to keep track of our score
public static var Score = 0;

// WARNING DO NOT USE A CONSTRUCTOR
//	     ALWAYS USE method Create()!!

// Create() is a built in function.
//	This will be called immediately after the state is added to the game.
method Create()
{
	// LoadScene() is a built in function
	LoadScene(Scenes.Game);
}

// Draw() is called every frame after Update()
// NOTE States' Draw() is called after all entitie's in the state have been drawn
method Draw()
{
	if (!Active)
	{
		// Draw a sprite when paused
		Sprite(Textures.Paused).Draw(800 / 2, 600 / 2);
		
		// NOTE Class("<resource>").Method() is a common shorthand for all kinds of things
		//	  You will see (and use) it for a lot of things.
		//	  Ex: Sound(Sounds.Bop).Play();
		//	  Ex: Sprite(Textures.HudOverlay).Draw();
	}
}

// FocusChanged() is called whenever the game window's gain or loses focus
method FocusChanged(var focus)
{
	// Active is a built-in variable.
	// When false NO entities will have their Update() called
	// Does not affect Draw()ing though, any code in those functions will still be called
	Active = focus;
}
