//!entity Rohan Rohan
// The above line is important for the scene editor
//	tells it that this is an entity.
//	This will be removed in the future, don't forget it
//	for now though.
//	MUST BE ON FIRST LINE
//	Must be exactly //!entity

// Declare a new entity "Rohan"
entity Rohan;

// Declare some private fields to remember the speed
private var xSpeed = 0.0;
private var ySpeed = 0.0;

// WARNING DO NOT USE A CONSTRUCTOR
//	     ALWAYS USE method Create()!!

// Create() is called after entity is added to state
method Create()
{
	// Sprite is a built in variable
	// Sprite() is like new Sprite()
	// If your entity has a sprite it MUST be set in Enter()
	Sprite = Sprite("Rohan");
	
	// Set a random position initially
	JumpToRandomPosition();
	
	// Set a random speed
	xSpeed = Random.Next() % 4 + 3.0;
	ySpeed = Random.Next() % 4 + 3.0;
	
	// BUG double % int works but double == int doesn't
	//	 Explicitly state doubles
	xSpeed = Random.Next() % 2 == 0.0 ? -xSpeed : xSpeed;
	ySpeed = Random.Next() % 2 == 0.0 ? -ySpeed : ySpeed;
}

// Update() is called every frame before Draw()
method Update()
{
	// Move the rohan
	X += xSpeed;
	Y += ySpeed;
	
	// Collide off the sides of the screen
	// BUG We need to explicitly use doubles instead of ints because the < and > operators
	//		won't compare doubles and ints together.
	if (X < 0.0 || X > 800.0 - 64.0)
		xSpeed = -xSpeed;
	
	if (Y < 0.0 || Y > 600.0 - 64.0)
		ySpeed = -ySpeed;
}

// MouseButtonPressed() is called when the entity is clicked on
method MouseButtonPressed(var button)
{
	// Check if the button clicked was the left button
	if (button == Mouse.Button.Left)
	{
		// Jump to a random position on the screen
		JumpToRandomPosition();
		
		// Also increase speed
		xSpeed *= 1.5;
		ySpeed *= 1.5;
	}
}

private method JumpToRandomPosition()
{
	X = Random.Next() % (800 - 64);
	Y = Random.Next() % (600 - 64);
}
