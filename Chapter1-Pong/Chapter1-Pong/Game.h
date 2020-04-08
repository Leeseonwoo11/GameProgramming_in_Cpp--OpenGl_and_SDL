#pragma once
#include "SDL.h"
#include <vector>


struct Vector2
{
	float x;
	float y;
};

struct Ball
{
	Vector2 BallPos;
	Vector2 BallVel;
};
using namespace std;
class Game
{
public:
	Game();

public:
	bool Initialize();
	void Runloop();
	void Shutdown();

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	SDL_Window* Window;
	SDL_Renderer* Renderer;
	bool IsRunning;
	Uint32 TickCount;

	int paddleDir;
	int paddleDir2;

	Vector2 PaddlePos;
	Vector2 PaddlePos2;

	vector<Ball>Ballvec;
	bool flag = true;

};



