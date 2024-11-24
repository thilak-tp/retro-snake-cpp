#include<iostream>
#include "game_engine.h"
// Using the standard namespace
using namespace std;

// This boolean is used to keep track if the game ends or not
bool gameOver;

// Width and height would take in the height and width of the game area
const width = 20;
const height = 20;

// These variable x and y to track the head of the snake and fruitX and fruitY is 
// to track the postion of the fruit that the snake eats
// score is to note the current total score of the game
int x,y,fruitX,fruitY, score;


// This enum is used for the directoin in which the snake moves
enum eDirection()
{
	STOP = 0;
  LEFT,
	RIGHT,
	UP,
	DOWN
};

eDirection dir;
// Driver Code
int main()
{
		
	return 0;

}


