#include "Game.h"
#include "SDL_image.h"
#include "Actor.h"
#include "SpriteComponent.h"
#include "BGSpriteCompoennt.h"
#include <algorithm>
#include "Ship.h"

Game::Game()
:Window(nullptr),
Renderer(nullptr),
bIsRunning(true),
bUpdatingActors(false)
{

}

bool Game::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		SDL_Log("Unable to initialize SDL : %s", SDL_GetError());
		return false;
	}
	Window = SDL_CreateWindow("Game Programming in c++ chapter2",
		10,
		10,
		1024,
		768,
		0);
	if (Window == nullptr)
	{
		SDL_Log("Window is nullptr %s", SDL_GetError());
		return false;
	}
	Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (Renderer == nullptr)
	{
		SDL_Log("Renderer is nullptr %s", SDL_GetError());
		return false;
	}
	if (IMG_Init(IMG_INIT_PNG) == 0)
	{
		SDL_Log("Unalbe to initialze SDL_image %s", SDL_GetError());
		return false;
	}
	LoadData();
	TickCount = SDL_GetTicks();
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
	SDL_DestroyRenderer(Renderer);
	SDL_DestroyWindow(Window);
	SDL_Quit();
}

void Game::AddActor(Actor * actor)
{
	if (!bUpdatingActors)
	{
		mActors.emplace_back(actor);
	}
	else
	{
		mPendingActors.emplace_back(actor);
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
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}
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
			SDL_Log("Failed to load texture file %s", SDL_GetError());
			return nullptr;
		}

		tex = SDL_CreateTextureFromSurface(Renderer, surf);
		SDL_FreeSurface(surf);
		if (!tex)
		{
			SDL_Log("Failed to covert surface to texture for %s", SDL_GetError());
			return nullptr;
		}
		mTexture.emplace(fileName.c_str(), tex); // mTexture�� unorderedmap ���Ŀ� �°� emplace�� �־��ش�. 
												// frist�� ���ڿ� second�� SDL_Texture* �ּ�
	}
	return tex;
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
	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE])
	{
		bIsRunning = false;
	}
	mShip->ProcessKeyboard(state);
}

void Game::UpdateGame()
{
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), TickCount + 16));

	float deltaTime = (SDL_GetTicks() - TickCount) / 1000.0f;
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}
	TickCount = SDL_GetTicks();
	bUpdatingActors = true;
	for (auto actor : mActors)
	{
		actor->Update(deltaTime);
	}
	bUpdatingActors = false;
	for (auto pending : mPendingActors)
	{
		mActors.emplace_back(pending);
	}
	mPendingActors.clear();

	std::vector<Actor*>deadActors;
	for (auto actor : mActors)
	{
		if (actor->GetState() == Actor::EDead)
		{
			deadActors.emplace_back(actor);
		}
	}
	for (auto actor : deadActors)
	{
		delete actor;
	}
}

void Game::GenerateOutput()
{
	SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 255);
	SDL_RenderClear(Renderer);

	for (auto sprite : mSprites) //mSprites�� ��������Ʈ ������Ʈ�� �����ɶ� AddSprite�Լ��� ���� �߰��ȴ�.
	{							// ��������Ʈ ������Ʈ�� �����Ϳ��� �Ѱ�, ship���Ϳ��� �Ѱ� �� 2���� ����/�߰� �ȴ�.
		sprite->Draw(Renderer); // �߰��� ������ ��������Ʈ������Ʈ���� ������ �°� �׷��ش�.
	}

	SDL_RenderPresent(Renderer); // ������ ����� ȭ�鿡 ����ִ� ����
}

void Game::LoadData()
{
	mShip = new Ship(this);
	mShip->SetPosition(Vector2(100.0f, 384.0f));
	mShip->SetScale(1.5f);
	
	Actor* temp = new Actor(this); // �����ͻ���
	temp->SetPosition(Vector2(512.0f, 384.0f));// ��������ġ����
	BGSpriteCompoennt* bg = new BGSpriteCompoennt(temp);
	bg->SetScreenSize(Vector2(1024.0f, 768.0f));
	std::vector<SDL_Texture*>bgtexs = { 
		GetTexture("Assets/Farback01.png"),
		GetTexture("Assets/Farback02.png")
	};
	bg->SetBGTextures(bgtexs);
	bg->SetScrollSpeed(-100.0f);

	bg = new BGSpriteCompoennt(temp, 50); // ����� �׸������ �⺻ 10�ε� ����޹�溸�ٴ� �ʰ� �׸��Ƿ� 50 ����
	bg->SetScreenSize(Vector2(1024.0f, 768.0f));
	bgtexs = {
		GetTexture("Assets/Stars.png"),
		GetTexture("Assets/Stars.png")
	};
	bg->SetBGTextures(bgtexs);
	bg->SetScrollSpeed(-200.0f);
	
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
