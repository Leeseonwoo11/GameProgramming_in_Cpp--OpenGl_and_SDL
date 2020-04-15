#include "Game.h"
#include "SDL_image.h"
#include <algorithm>
#include "Actor.h"
#include "SpriteComponent.h"
#include "Ship.h"
#include "Asteroid.h"
#include "Random.h"


Game::Game():mWindow(nullptr),
mRenderer(nullptr),
bIsRunning(true),
bUpdatingActors(false)
{
}

bool Game::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		SDL_Log("Unable to iniitialize SDL :  %s", SDL_GetError());
		return false;
	}
	mWindow = SDL_CreateWindow("GameProgramming in c++ chapter3",
		10,
		10,
		1024,
		768,
		0);
	if (mWindow == nullptr)
	{
		SDL_Log("Failed to Create window : %s", SDL_GetError());
		return false;
	}
	
	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
	if (mRenderer == nullptr)
	{
		SDL_Log("Failed to Create Renderer: %s ", SDL_GetError());
		return false; 
	}
	if (IMG_Init(IMG_INIT_PNG) == 0)
	{
		SDL_Log("Unable to initialize SDL_image : %s", SDL_GetError());
		return false;
	}
	Random::Init();

	LoadData();

	mTeicksCounts = SDL_GetTicks();

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

void Game::Shutdown()
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
	if (iter != mActors.end())
	{
		std::iter_swap(iter, mActors.end() - 1 );
		mActors.pop_back();
	}
}

void Game::AddSprite(SpriteComponent * sprite)
{
	int myDrawOrder = sprite->GetDrawOrder();
	auto iter = mSprites.begin();
	for (; iter != mSprites.end();iter++)
	{
		if (myDrawOrder < (*iter)->GetDrawOrder())
		{
			break;
		}
	}
	mSprites.insert(iter, sprite);
}

void Game::RemoveSprite(SpriteComponent * sprite)
{
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	mSprites.erase(iter);
}

SDL_Texture * Game::GetTexture(const std::string & fileName)
{
	SDL_Texture* tex = nullptr;
	auto iter = mTextures.find(fileName);
	if (iter != mTextures.end())
	{
		tex = iter->second;
	}
	else
	{
		SDL_Surface* surf = IMG_Load(fileName.c_str());
		if (!surf)
		{
			SDL_Log("Failed to load texture file %s", SDL_GetError());
			return nullptr;
		}
		tex = SDL_CreateTextureFromSurface(mRenderer, surf);
		SDL_FreeSurface(surf);
		if (!tex)
		{
			SDL_Log("Failed to convert surface to texture for %s ", SDL_GetError());
			return nullptr;
		}
		mTextures.emplace(fileName.c_str(), tex);
	}
	return tex;
}

void Game::AddAsteroid(Asteroid * ast)
{
	mAsteroids.emplace_back(ast);
}

void Game::RemoveAsteroid(Asteroid* ast)
{
	auto iter = std::find(mAsteroids.begin(),
		mAsteroids.end(), ast);

	if (iter != mAsteroids.end())
	{
		mAsteroids.erase(iter);
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

	const Uint8* KeyState = SDL_GetKeyboardState(NULL);
	if (KeyState[SDL_SCANCODE_ESCAPE])
	{
		bIsRunning = false;
	}
	bUpdatingActors = true;
	for (auto actor : mActors)
	{
		actor->ProcessInput(KeyState);
	}
	bUpdatingActors = false;

}

void Game::UpdateGame()
{
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTeicksCounts + 16));

	float deltatime = (SDL_GetTicks() - mTeicksCounts) / 1000.0f;
	if (deltatime > 0.05f)
	{
		deltatime = 0.05f;
	}
	mTeicksCounts = SDL_GetTicks();
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

	std::vector<Actor*>DeadActors;
	for (auto actor : mActors)
	{
		if (actor->GetState() == Actor::EDead)
		{
			DeadActors.emplace_back(actor);
		}
	}

	for (auto actor : DeadActors)
	{
		delete actor;
	}
}

void Game::GenerateOutput()
{
	SDL_SetRenderDrawColor(mRenderer, 200, 220, 220, 255);
	SDL_RenderClear(mRenderer);
	for (auto sprite : mSprites)
	{
		sprite->Draw(mRenderer);
	}
	SDL_RenderPresent(mRenderer);
}

void Game::LoadData()
{
	mShip = new Ship(this);
	mShip->SetPosition(Vector2(1024.0f / 2.0f, 768.0f / 2.0f));
	mShip->SetRotation(Math::PiOver2);

	const int numAsteroids = 40;
	for (int i = 0; i < numAsteroids; i++)
	{
		new Asteroid(this);
	}
}

void Game::UnloadData()
{
	while (!mActors.empty())
	{
		delete mActors.back();
	}
	for (auto tex : mTextures)
	{
		SDL_DestroyTexture(tex.second); // 언오더드맵의 뒤는 SDL_Texture*로 이루어져있어 먼저 지워주고 언오더드맵을 클리어해서 지워준다.
	}
	mTextures.clear();
}
