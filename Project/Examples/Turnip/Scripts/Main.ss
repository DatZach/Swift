/*
 *	Main.ss
 *	Primary state
 *	IMPORTANT NOTES!
 *		
 *	NOTES
 *		The window size was changed in the Configuration
 *		SOUNDS ARE NOT COPIED CORRECTLY SO MUSIC WILL NOT PLAY UNLESS
 *		YOU COPY THE FILE INTO THE SAME DIRECTORY AS SWIFT (IDE, Compiler and Engine)
 *		
 *		Also note that I didn't do this in the redist, to experience the example correctly
 *		Follow these steps:
 *			1. Click Asssets -> Open Project in Explorer
 *			2. Double click the "Sounds" folder
 *			3. Copy "turnup.ogg" into the clipboard
 *			4. Click File -> Open Swift in Explorer
 *			5. Past turnup.ogg
 *		Should look like this in the end http://i.imgur.com/xnDIH8Q.png
 *		Notice that turnup.ogg is in the same directory as the IDE.
 *
 *		THIS WILL BE FIXED SOONISH.
 */

// Default state
state Main;

// Keep track of the time we started the game
var startTime;

// Enter() is called automatically when the state is entered
method Enter()
{
	// Time is a builtin static class
	// Time.GetTimestamp() will return the current UNIX timestamp (seconds)
	startTime = Time.GetTimestamp();
	
	// In C# this is the same as new Sound("TurnUp").Play()
	// Play the sound "Turn Up"
	Sound("TurnUp").Play();
}

// Draw() gets called every frame after Update()
method Draw()
{
	// Draw background ourselves since we don't have a scene to do that for us
	Sprite("Background").Draw();

	// Calculate how many seconds it's been since the start of the game/song
	var seconds = Time.GetTimestamp() - startTime;
	
	// Draw a turnip every other second after 40 seconds for 26 seconds
	if (seconds >= 40 && seconds < 64 && (seconds % 2) == 0)
		Sprite("Turnip").DrawDD(160.0, 56.0);
}
