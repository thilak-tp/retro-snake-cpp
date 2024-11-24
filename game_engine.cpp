
void Setup()
{
	// This covers initialization of the game.
	// Since the game is gonna begin hence gameOver flag is set to false
	gameOver = false;
	// The direction enum is to note the direction in which the snake has to move
	dir = STOP;
	// The coodinate of the snakes head at the begin is said to be in the middle of 
  // of the play area i.e half of the width and half of the height
  x = width/2;
  y = height/2;

	// The poistion of the fruit is to be randomized with respect to the width and
  // height of the play area
	friutX = rand() % width;
  fruitY = rand() % height;

}
void Draw()
{

}
void Input()
{

}
void Logic()
{


}
