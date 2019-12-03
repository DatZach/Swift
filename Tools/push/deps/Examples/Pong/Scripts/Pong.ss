state Pong
{
	ClearColor = Color(0, 0, 0)
};

public static var Player1Score = 0;
public static var Player2Score = 0;

private var chars;

method()
{
	chars = [ Textures.Zero, Textures.One, Textures.Two, Textures.Three, Textures.Four,
			  Textures.Five, Textures.Six, Textures.Seven, Textures.Eight, Textures.Nine ];
}

method Create()
{
	Add(Paddle(16, 600 / 2 - 96 / 2, Paddle.PlayerIds.First));
	Add(Paddle(800 - 24 - 16, 600 / 2 - 96 / 2, Paddle.PlayerIds.Second));
	Add(Ball());
}

method Draw()
{
	DrawText(str(Pong.Player1Score), 800 / 2 - 12, 24, 0);
	DrawText(str(Pong.Player2Score), 800 / 2 + 12, 24, 1);
}

private method DrawText(var text : str, var x, var y, var align)
{
	if (!align)
		x -= 64;
	
	foreach(var c in text)
	{
		Sprite(chars[int(c)]).Draw(x, y);
		x += align ? 64 : -64;
	}
}
