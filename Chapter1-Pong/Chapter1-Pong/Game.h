#pragma once
#include "SDL/SDL.h"

struct Vector2
{
	float x;
	float y;
};

class Game
{
public:
	Game();
	~Game();
	bool Initialize();
	void RunLoop();
	void Shutdown();
private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	SDL_Window* Window;
	SDL_Renderer* Renderer;
	Uint32 TicksCount;
	bool bIsRunning;
	int PaddleDir;
	Vector2 PaddlePos;
	Vector2 BallPos;
	Vector2 BallVel;

};

