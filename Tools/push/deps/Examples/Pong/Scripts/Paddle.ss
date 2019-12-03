entity Paddle
{
	Sprite = Sprite(Textures.Paddle)
};

private const PaddleSpeed = 5;

public enum PlayerIds
{
	First,
	Second
};

public property PlayerId { get; private set; }

method(var xx, var yy, var playerId)
{
	PlayerId = playerId;
	x = xx;
	y = yy;
}

method KeyHeld(var key)
{
	var upKey	= PlayerId == PlayerIds.First ? Keyboard.Key.W : Keyboard.Key.Up;
	var downKey	= PlayerId == PlayerIds.First ? Keyboard.Key.S : Keyboard.Key.Down;
	
	if (key == upKey)
		y -= PaddleSpeed;
	else if (key == downKey)
		y += PaddleSpeed;
	
	if (y < 0)
		y = 0;
	else if (y > 600 - 96)
		y = 600 - 96;
}
