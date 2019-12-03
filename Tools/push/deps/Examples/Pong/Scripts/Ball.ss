entity Ball
{
	Sprite = Sprite(Textures.Ball)
};

private const BallSpeed = 6;

private var xSpeed;
private var ySpeed;

method Create()
{
	ResetBallPosition();
}

method Update()
{
	x += xSpeed;
	y += ySpeed;
	
	if (GetCollidingFirst() != null)
		xSpeed = -xSpeed;
		
	if (y < 12 || y > 600 - 12)
		ySpeed = -ySpeed;
	
	if (x < -24 || x > 824)
		Score();
}

method Score()
{
	if (x > 400)
		Pong.Player1Score += 1;
	else
		Pong.Player2Score += 1;

	ResetBallPosition();
}

private method ResetBallPosition()
{
	x = 800 / 2;
	y = 600 / 2;
	
	xSpeed = Random.Choose(BallSpeed, -BallSpeed);
	ySpeed = Random.Choose(BallSpeed, -BallSpeed);
}
