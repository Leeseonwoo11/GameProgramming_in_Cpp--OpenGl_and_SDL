#pragma once
#include "SDL.h"
#include <unordered_map>
#include <string>
#include <vector>
#include "Math.h"

class Game
{
public:
	Game();
	bool Initialize();
	void RunLoop();
	void ShutDown();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);

	SDL_Texture* GetTexture(const std::string& fileName);
	class Grid* GetGrid() { return mGrid; }
	std::vector<class Enemy*>& GetEnemies() { return mEnemies; }
	class Enemy* Getnearestenemy(const Vector2& pos);

	void CountingGoalEnemy();

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();

	std::unordered_map<std::string, SDL_Texture*> mTexture;
	std::vector<class Actor*> mActors;
	std::vector<class Actor*>mPendingActors;

	std::vector<class SpriteComponent*> mSprites;

	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	Uint32 mTickCount;
	bool bIsRunning;
	bool bUpdatingActors;

	std::vector<class Enemy*> mEnemies;
	class Grid* mGrid;
	float mNexEnemy;


	class Actor* a;
	class AIComponent* aic;
	int EnemyGoalCount = 0;
};

