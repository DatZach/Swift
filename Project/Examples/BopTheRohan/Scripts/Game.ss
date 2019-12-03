/*
 *	Game.ss
 *	Main game state
 *	IMPORTANT NOTES!
 *		YOU MUST CREATE AT LEAST ONE OF THESE
 *		YOU MUST SPECIFY AN INITIAL STATE IN "Configuration"
 *
 *	NOTES
 *		Semicolons are optional
 *		"state Game" is just as valid as "state Game;"
 */

// Declare this as a state
state Game;

// WARNING DO NOT USE A CONSTRUCTOR
//	     ALWAYS USE method Create()!!

// Enter() is a built in function.
//	This will be called immediately after the state is added to the game.
method Enter()
{
	// !!!!IMPORTANT!!! YOU MUST ADD THIS LINE BEFORE LOADING A SCENE
	// It's a bug and will cause the engine to CRASH. Will be fixed soon.
	// ClearColor is a built in variable
	ClearColor = Color(0, 0, 0, 0);
	
	// Load our scene (DO NOT INCLUDE EXTENSION)
	// LoadScene() is a built in function
	LoadScene("Game");
}

// Draw() is called every frame after Update()
// NOTE States' Draw() is called after all entitie's in the state have been drawn
method Draw()
{
	if (!Active)
	{
		// Draw a sprite when paused
		// BUG DrawDD(x, y) should accept ints too, explicitly state doubles
		Sprite("Paused").DrawDD((800.0 - 640.0) / 2.0, (600.0 - 320.0) / 2.0);
		
		// NOTE Sprite origins don't work yet so the above code just centers the texture on the screen
		// NOTE Class("<resource>").Method() is a common shorthand for all kinds of things
		//	  You will see (and use) it for a lot of things.
		//	  Ex: Sound("Bop").Play();
		//	  Ex: Sprite("HudOverlay").Draw();
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
