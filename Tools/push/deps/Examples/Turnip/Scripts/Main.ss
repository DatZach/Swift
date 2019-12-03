/*
 *	Main.ss
 *	Primary state	
 *	NOTES
 *		The window size was changed in the Configuration
 */

// Default state
state Main;

// Keep track of the time we started the game
var startTime;

// Create() is called automatically when the state is entered
method Create()
{
	// Time is a builtin static class
	// Time.GetTimestamp() will return the current UNIX timestamp (seconds)
	startTime = Time.Timestamp;
	
	// In C# this is the same as new Sound("TurnUp").Play()
	// Play the sound "Turn Up"
	Sound(Sounds.TurnUp).Play();
}

// Draw() gets called every frame after Update()
method Draw()
{
	// Draw background ourselves since we don't have a scene to do that for us
	Sprite(Textures.Background).Draw();

	// Calculate how many seconds it's been since the start of the game/song
	var seconds = Time.Timestamp - startTime;
	
	// Draw a turnip every other second after 40 seconds for 26 seconds
	if (seconds >= 40 && seconds < 64 && (seconds % 2) == 0)
		Sprite(Textures.Turnip).Draw(160, 56);
}
