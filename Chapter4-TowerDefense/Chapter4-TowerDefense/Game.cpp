#include "Game.h"
#include "SDL_image.h"
#include <algorithm>
#include "Actor.h"
#include "SpriteComponent.h"
#include "Grid.h"
#include "Enemy.h"
#include "AIComponent.h"
#include "AIState.h"

Game::Game()
	:mWindow(nullptr),
	mRenderer(nullptr),
	bIsRunning(true),
	bUpdatingActors(true)
{
}

bool Game::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		SDL_Log("Unable to initialize SDL : %s", SDL_GetError());
		return false;
	}
	mWindow = SDL_CreateWindow("Gameprogramming in c++ chapter4", 10, 10, 1024, 768, 0);
	if (mWindow == nullptr)
	{
		SDL_Log("Failed to create window %s", SDL_GetError());
		return false;
	}
	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (mRenderer == nullptr)
	{
		SDL_Log("Failed to create renderer %s", SDL_GetError());
		return false;
	}

	if (IMG_Init(IMG_INIT_PNG) == 0)
	{
		SDL_Log("Unalbe to initialize SDL_image %s", SDL_GetError());
		return false;
	}
	LoadData();
	mTickCount = SDL_GetTicks();
	return true;
}

void Game::RunLoop()
{
	while (bIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			bIsRunning = false;
			break;
		}
	}
	const Uint8* keystate = SDL_GetKeyboardState(NULL);
	if (keystate[SDL_SCANCODE_ESCAPE])
	{
		bIsRunning = false;
	}
	if (keystate[SDL_SCANCODE_B])
	{
		mGrid->BuildTower();
	}
	if (keystate[SDL_SCANCODE_1])
	{
		aic->ChangeState("Patrol");
	}
	if (keystate[SDL_SCANCODE_2])
	{
		aic->ChangeState("Death");
	}
	if (keystate[SDL_SCANCODE_3])
	{
		aic->ChangeState("Attack");
	}
	int x, y;
	Uint32 buttons = SDL_GetMouseState(&x, &y);
	if (SDL_BUTTON(buttons) & SDL_BUTTON_LEFT)
	{
		mGrid->ProcessClick(x, y);
	}

	bUpdatingActors = true;
	for (auto actor : mActors)
	{
		actor->ProcessInput(keystate);
	}
	bUpdatingActors = false;
}

void Game::UpdateGame()
{
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTickCount + 16));
	float deltatime = (SDL_GetTicks() - mTickCount) / 1000.0f;
	if (deltatime > 0.05f)
	{
		deltatime = 0.05f;
	}
	mTickCount = SDL_GetTicks();
	bUpdatingActors = true;
	for (auto actor : mActors)
	{
		actor->Update(deltatime);
	}
	bUpdatingActors = false;

	for (auto pending : mPendingActors)
	{
		mActors.emplace_back(pending);
	}
	mPendingActors.clear();

	std::vector<Actor*> deadActor;
	for (auto actor : mActors)
	{
		if (actor->GetState() == Actor::EDead)
		{
			deadActor.emplace_back(actor);
		}
	}
	for (auto actor : deadActor)
	{
		delete actor;
	}
}

void Game::GenerateOutput()
{
	SDL_SetRenderDrawColor(mRenderer, 34, 139, 34, 255);
	SDL_RenderClear(mRenderer);

	for (auto sprite : mSprites)
	{
		sprite->Draw(mRenderer);
	}
	SDL_RenderPresent(mRenderer);
}

void Game::LoadData()
{
	mGrid = new Grid(this);// 타일들을 가지고 있는 그리드 클래스
	
	// AI state를 적용하고 로그를 띄워보기 위해서 초기화하였다.
	a = new Actor(this);
	aic = new AIComponent(a);
	aic->RegisterState(new AIPatrol(aic));
	aic->RegisterState(new AIDeath(aic));
	aic->RegisterState(new AIAttack(aic));

	aic->ChangeState("Patrol");

}

void Game::UnloadData()
{
	while (!mActors.empty())
	{
		delete mActors.back();
	}
	for (auto i : mTexture)
	{
		SDL_DestroyTexture(i.second);
	}
	mTexture.clear();
}

void Game::AddSprite(SpriteComponent * sprite)
{
	int myDrawOrder = sprite->GetDrawOrder();
	auto iter = mSprites.begin();
	for (; iter != mSprites.end(); iter++)
	{
		if (myDrawOrder < (*iter)->GetDrawOrder())
		{
			break;
		}
	}
	mSprites.insert(iter,sprite);
}

void Game::RemoveSprite(SpriteComponent * sprite)
{
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	mSprites.erase(iter);
}

SDL_Texture * Game::GetTexture(const std::string & fileName)
{
	SDL_Texture* tex = nullptr;
	auto iter = mTexture.find(fileName);
	if (iter != mTexture.end())
	{
		tex = iter->second;
	}
	else
	{
		SDL_Surface* surf = IMG_Load(fileName.c_str());
		if (!surf)
		{
			SDL_Log("Failedd to load texture file %s", fileName.c_str());
			return nullptr;
		}
		tex = SDL_CreateTextureFromSurface(mRenderer, surf);
		SDL_FreeSurface(surf);
		if (!tex)
		{
			SDL_Log("Failed to convert surfasce to texture for %s", fileName.c_str());
			return nullptr;
		}
		mTexture.emplace(fileName.c_str(), tex);
	}
	return tex;
}

void Game::ShutDown()
{
	UnloadData();
	IMG_Quit();
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

void Game::AddActor(Actor * actor)
{
	if (bUpdatingActors)
	{
		mPendingActors.emplace_back(actor);
	}
	else
	{
		mActors.emplace_back(actor);
	}
}

void Game::RemoveActor(Actor * actor)
{
	auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
	if (iter != mPendingActors.end())
	{
		std::iter_swap(iter, mPendingActors.end() - 1);
		mPendingActors.pop_back();
	}
	iter = std::find(mActors.begin(), mActors.end(), actor);
	if(iter != mActors.end())
	{
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}
}

Enemy * Game::Getnearestenemy(const Vector2 & pos)
{
	Enemy* best = nullptr;
	if (mEnemies.size() > 0)
	{
		best = mEnemies[0];
		float bestDistq = (pos - mEnemies[0]->GetPosition()).LengthSq();
		for (size_t i = 1; i < mEnemies.size(); i++)
		{
			float newDistSq = (pos - mEnemies[i]->GetPosition()).LengthSq();
			if (newDistSq < bestDistq)
			{
				bestDistq = newDistSq;
				best = mEnemies[i];
			}
		}
	}
	return best;
}

void Game::CountingGoalEnemy() //목표에 적이 들어서면 실행된다.
{
	EnemyGoalCount++; // 골에 들어간 적 수 +1
	if(EnemyGoalCount == 10)
		bIsRunning = false;
}
