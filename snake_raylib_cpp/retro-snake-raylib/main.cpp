#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include <deque>

using namespace std;

// Defining the color structs for the game UI
Color green = {172, 204, 96, 255};
Color darkGreen = { 43, 51, 24, 255 };

// This is used for griding on the area
int cellSize = 30;
int cellCount = 25;

// This variable is to offset and draw the inner rectangular area
int offset = 75;

double lastUpdateTime = 0;

// Check if the vector is there in the Deque or not
bool ElementInDeque(Vector2 element, deque<Vector2> deq)
{
	for (unsigned int i = 0; i < deq.size(); i++)
	{
		if (Vector2Equals(element, deq[i]))
		{
			return true;
		}

	}
	return false;
}
// A function to check if the time has elapsed for the movement of the snake to be slower than 60FPS
bool eventTriggered(double interval)
{
	double currentTime = GetTime();
	if (currentTime - lastUpdateTime >= interval)
	{
		lastUpdateTime = currentTime;
		return true;
	}
	return false;
}
//Defining the Snake class
class Snake
{
public:
	// A deque is a double ended queue that is used here for the head and tail of the snake
	deque<Vector2> body = { Vector2{6,9},Vector2{5,9},Vector2{4,9} };

	// This is to check for the direction of the snake
	Vector2 direction = { 1,0 };

	//This is to check if we have added a segment to the snake or not
	bool addSegment = false;

	// To draw the snake on the canvas
	void Draw()
	{
		
		for (unsigned int i = 0; i < body.size(); i++)
		{
			float x = body[i].x;
			float y = body[i].y;
			Rectangle segment = Rectangle{offset + x*cellSize,offset + y*cellSize, (float)cellSize, (float)cellSize};
			DrawRectangleRounded(segment, 0.5, 6, darkGreen );

		}
	}

	void Update()
	{
		// Add a segment to the snake in the direction else pop at the back of the deque
		body.push_front(Vector2Add(body[0], direction));
		if (addSegment == true)
		{
			addSegment = false;
		}
		else
		{ 
			body.pop_back();	
		}
	}

	// A function to reset the game when it ends
	void Reset()
	{
		// Resetting the snake size to three and also resetting the direction of the snake
		body = { Vector2{6,9}, Vector2{5,9}, Vector2{4,9} };
		direction = { 1,0 };
	}
};

//Defining the food class
class Food
{
public:
	Vector2 position;
	// A texture object is created to map the food image 
	Texture2D texture;

	// A food constructor that does init tasks for the object
	Food(deque<Vector2> snakeBody)
	{
		// A food image is loaded as food and then textured
		Image image = LoadImage("food.png");
		texture = LoadTextureFromImage(image);
		UnloadImage(image);
		// This position will show the position of food which is randomized
		position = GenerateRandomPos(snakeBody);
	}
	
	

	~Food()
	{
		UnloadTexture(texture);
	}

	Vector2 GenerateRandomCell()
	{
		//To randomly place the food anywhere in the play area
		float x = GetRandomValue(0, cellCount - 1);
		float y = GetRandomValue(0, cellCount - 1);
		return Vector2{x,y};
	}
	Vector2 GenerateRandomPos(deque<Vector2> snakeBody)
	{
		// This is to not spawn food on the snakes body
		Vector2 position = GenerateRandomCell();
		while (ElementInDeque(position, snakeBody))
		{
			position = GenerateRandomCell();
		}
		return position;
	}

	void Draw()
	{
		// This draws the food on the canvas
		//DrawRectangle(position.x*cellSize, position.y * cellSize, cellSize, cellSize, darkGreen);
		DrawTexture(texture, offset + position.x * cellSize, offset + position.y * cellSize, WHITE);
	}
};

// The overall class
class Game
{
public:
	Snake snake = Snake();
	Food food = Food(snake.body);
	// To check if the game is running or paused

	bool running = true;
	// To add the scoring system
	int score = 0;
	// To include collision sounds
	Sound eatSound;
	Sound WallSound;

	// The Game class constructor to load the sounds
	Game()
	{
		InitAudioDevice();
		eatSound = LoadSound("eat.mp3");
		WallSound = LoadSound("wall.mp3");
	}

	// The Game class destructor to unload the sounds
	~Game()
	{
		UnloadSound(eatSound);
		UnloadSound(WallSound);
		CloseAudioDevice();
	}

	// The game Draw function calls the food and snake Draw functions
	void Draw()
	{
		food.Draw();
		snake.Draw();
	}
	
	void Update()
	{
		// Only when the running flag is true, do things else don't
		if (running)
		{
			snake.Update();
			CheckCollisionWithFood();
			CheckCollisionWithEdges();
			CheckCollisionWithTail();
		}
	}

	// A function to check if the food is being eaten by snake
	void CheckCollisionWithFood()
	{
		if (Vector2Equals(snake.body[0], food.position))
		{
			cout << "Eating food..." << endl;
			food.position = food.GenerateRandomPos(snake.body);
			snake.addSegment = true;
			score += 10;
			PlaySound(eatSound);
		}
	}
	
	// A function to check if there is a collision of the snake to the walls so to exit the game
	void CheckCollisionWithEdges()
	{
		if (snake.body[0].x == cellCount || snake.body[0].x == -1)
		{
			GameOver();
			PlaySound(WallSound);
		}
		if(snake.body[0].y == cellCount || snake.body[0].y == -1)
		{
			GameOver();
			PlaySound(WallSound);
		}
		
	}

	// To check for the collision of the snake with its own tail 
	void CheckCollisionWithTail()
	{
		deque<Vector2> headlessbody = snake.body;
		headlessbody.pop_front();
		if (ElementInDeque(snake.body[0], headlessbody))
		{
			GameOver();
		}
	}
	// This function handles everything when the game is over
	void GameOver()
	{
		cout << "Game over...\n";
		// When the game is over, the snake size is reset to 3 blocks, food is again randomized and running flag is kept false
		snake.Reset();
		food.position = food.GenerateRandomPos(snake.body);
		running = false;
		// The score is reset to zero when the game ends
		score = 0;
	}
	
};

int main()
{
	cout << "Starting the game...." << endl;

	// Defining the window size along with the window title
	InitWindow(2*offset + cellSize*cellCount, 2 * offset + cellSize * cellCount, "Retro Snake");

	// Predefining the Frame per second at which this will be rendered
	SetTargetFPS(60);
	
	Game game = Game();
	// The game loop
	// Here WindowShouldClose() is used to check for Window close
	// or for Esc being pressed on the the keyboard
	while (WindowShouldClose() == false)
	{
		//This fn draws the window on the screen
		BeginDrawing();

		//Only update the postion of the snake every 200ms only instead of the 60 times per second
		if (eventTriggered(0.2))
			game.Update();

		//Clear background with the green color we created
		ClearBackground(green);
		DrawRectangleLinesEx(Rectangle{ float(offset - 5), float(offset - 5), float(cellSize * cellCount + 10), float(cellSize * cellCount + 10) }, 5, darkGreen);

		//The following code checks for key presses and also checks if it going in one direction it can't immediately go in the opposite direction
		if (IsKeyPressed(KEY_UP) && game.snake.direction.y != 1)
		{
			game.snake.direction = { 0, -1 };
			game.running = true;
		}

		if (IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1)
		{
			game.snake.direction = { 0, 1 };
			game.running = true;
		}
		if (IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1)
		{
			game.snake.direction = { -1,0 };
			game.running = true;
		}
		if (IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1)
		{
			game.snake.direction = { 1, 0 };
			game.running = true;
		}
		
		// Draw the title in the game area
		DrawText("Retro Snake", offset - 5, 20, 30, darkGreen);
		// To display the score on the screen
		DrawText(TextFormat("%i",game.score), offset - 5, offset + cellSize * cellCount + 10,30, darkGreen);
		//Calling the snake draw function
		game.Draw();

		//This function closes the windows on the screen
		EndDrawing();
	}


	CloseWindow();
	return 0;
}