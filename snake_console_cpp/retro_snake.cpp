#include<iostream>
#include<windows.h>// for windows
//#include<unistd.h>// for linux
#include<conio>
using std::cout;
using std::cin;
using std::endl;
using std::rand;

// This boolean is used to keep track if the game ends or not
bool gameOver;

// Width and height would take in the height and width of the game area
const int width = 20;
const int height =20;

// These variable x and y to track the head of the snake and fruitX and fruitY is
// to track the postion of the fruit that the snake eats
// score is to note the current total score of the game
int x,y,fruitX,fruitY, score;
// For the tail of the snake
int tailX[100], tailY[100];
int nTail;


// This enum is used for the directoin in which the snake moves
enum eDirection
{
  STOP = 0,
  LEFT,
  RIGHT,
  UP,
  DOWN
};

eDirection dir;
// Driver Code



void Setup();
void Draw();
void Input();
void Logic();
int main()
{

	Setup();
	while (!gameOver)
	{
		Draw();
		Input();
		Logic();
	}

	return 0;

}


void Setup()
{
	// This covers initialization of the game.
	// Since the game is gonna begin hence gameOver flag is set to false
	gameOver = false;
	// The direction enum is to note the direction in which the snake has to move
	dir = STOP;
	// The coodinate of the snakes head at the begin is said to be in the middle of
	// of the play area i.e half of the width and half of the height
	x = width / 2;
	y = height / 2;

	// The poistion of the fruit is to be randomized with respect to the width and
	// height of the play area
	fruitX = rand() % width;
	fruitY = rand() % height;
	nTail = 0;

	score = 0;
}

void Draw()
{
	// Adjust sleep for fine tuning flickering of the screen
	Sleep(50);
	//This is to clear the screen
	//Use cls for windows systems and clear for linux systems
	system("cls");

	// This loop is to print the upper width of the rectangular play area
	for (int i = 0; i < width + 2; i++)
		cout << "#";

	cout << "\n";

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			// For the starting character of the lines other than the first and the last line
			if (j == 0)
				cout << "#";
			// For the position of the snakes head in the play area
			if (i == y && j == x)
				cout << "O";

			// For the postion of the fruit in the play area
			else if (j == fruitX && i == fruitY)
				cout << "@";
			// For the spaces in the play area other than the first line and the second line
			else
			{
				bool print = false;
				for (int k = 0; k < nTail; k++)
				{
					// When the coodinate is tailx of k and taily of k, print the tail	
					if (tailX[k] == j && tailY[k] == i)
					{
						cout << "o";
						// Check if the tail is printed, if printed put the print flag as true
						print = true;
					}
					
				}
				// If the printed then dont print space else print space
				if (!print)
					cout << " ";
			}
			// For the ending character of the lines other than the first and the last line
			if (j == width - 1)
				cout << "#";
		}
		cout << "\n";
		
	}

	// This loop is to print the lower width of the rectangular play area
	for (int i = 0; i < width + 2; i++)
		cout << "#";
	cout << "\n";
	cout << "Score = " << score << "\n";

}
void Input()
{
	// Read for key presses
	if (_kbhit())
	{
		// Switch for each key press
		switch (_getch())
		{
		case 'a': dir = LEFT;
			break;
		case 's': dir = DOWN;
			break;
		case 'w': dir = UP;
			break;
		case 'd': dir = RIGHT;
			break;
		case 'x': 
			gameOver = true;
			break;
	    
		}
	}

}
void Logic()
{
  //Logic for the tail incrementing when eating food
	int prevX = tailX[0];
	int prevY = tailY[0];
	int prev2X, prev2Y;
	tailX[0] = x;
	tailY[0] = y;
	for (int i = 1; i < nTail; i++)
	{
		prev2X = tailX[i];
		prev2Y = tailY[i];
		tailX[i] = prevX;
		tailY[i] = prevY;
		prevX = prev2X;
		prevY = prev2Y;
	}
	// for changing the direction of the snake
	switch (dir)
	{
	case LEFT:
		x--;
		break;
	case RIGHT:
		x++;
		break;
	case UP:
		y--;
		break;
	case DOWN:
		y++;
		break;
	default: 
		break;
	}

	// If we move over the play area or hit a wall, game over
	if (x > width || x < 0 || y < 0 || y > height)
		gameOver = true;
	for (int i = 0; i < nTail; i++)
	{
		if (tailX[i] == x && tailY[i] == y)
		{
			gameOver = true;
		}
	}
	if (x == fruitX && y == fruitY)
	{
		score += 10;
		fruitX = rand() % width;
		fruitY = rand() % height;
		nTail++;

	}

}
