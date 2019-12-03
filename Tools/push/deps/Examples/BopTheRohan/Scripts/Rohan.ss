//!entity Rohan Rohan
// The above line is important for the scene editor
//	tells it that this is an entity.
//	This will be removed in the future, don't forget it
//	for now though.
//	MUST BE ON FIRST LINE
//	Must be exactly //!entity

// Declare a new entity "Rohan"
entity Rohan
{
	// Initialize ENTITY or STATE type specifics here
	// Putting them in the initializer list will always ensure that they are
	// created correctly. The Scene Editor also reads this list to determine
	// what attributes to use for entities.
	Sprite = Sprite(Textures.Rohan)
};

// Declare some private fields to remember the speed
private var xSpeed = 0.0;
private var ySpeed = 0.0;

// WARNING DO NOT USE A CONSTRUCTOR
//	     ALWAYS USE method Create()!!

// Create() is called after entity is added to state
method Create()
{	
	// Set a random position initially
	JumpToRandomPosition();
	
	// Set a random speed
	xSpeed = Random.Next() % 4 + 3;
	ySpeed = Random.Next() % 4 + 3;
	xSpeed = Random.Choose(-xSpeed, xSpeed);
	ySpeed = Random.Choose(-ySpeed, ySpeed);
}

// Update() is called every frame before Draw()
method Update()
{
	// Move the rohan
	x += xSpeed;
	y += ySpeed;
	
	// Collide off the sides of the screen
	if (x < 0 || x > 800 - 64)
		xSpeed = -xSpeed;
	
	if (y < 0 || y > 600 - 64)
		ySpeed = -ySpeed;
}

// MouseButtonPressed() is called when the entity is clicked on
method MouseButtonPressed(var button)
{
	// Check if the button clicked was the left button
	if (button == Mouse.Button.Left)
	{
		Sound(Sounds.Boop).Play();
		
		// Jump to a random position on the screen
		JumpToRandomPosition();
		
		// Also increase speed
		xSpeed *= 1.5;
		ySpeed *= 1.5;
		
		// Increment score and display it
		Primary.Score += 1;
		Debug.WriteLine("Score: " + str(Primary.Score));
	}
}

private method JumpToRandomPosition()
{
	x = Random.Next() % (800 - 64);
	y = Random.Next() % (600 - 64);
	xSpeed = Random.Choose(-xSpeed, xSpeed);
	ySpeed = Random.Choose(-ySpeed, ySpeed);
}
