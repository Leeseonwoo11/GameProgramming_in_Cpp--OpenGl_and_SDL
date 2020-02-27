#include "pch.h"
#include "Game.h"


const int thickness = 15;
const float paddleH = 100.0f;

Game::Game():Window(nullptr),Renderer(nullptr),TicksCount(0),bIsRunning(true),PaddleDir(0)
{
}

Game::~Game()
{
}

bool Game::Initialize()
{
	PaddlePos.x = 10.0f;
	PaddlePos.y = 768.0f / 2.0f;
	BallPos.x = 1024.0f / 2.0f;
	BallPos.y = 768.0f / 2.0f;
	BallVel.x = -200.0f;
	BallVel.y = 235.0f;


	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult != 0)
	{
		SDL_Log("Failed to create window: %s",SDL_GetError());
		return false;
	}
	Window = SDL_CreateWindow(
		"GameProgramming Pong-1",
		100,
		100,
		1024,
		768,
		0);
	if (Window == nullptr)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}
	Renderer = SDL_CreateRenderer(
		Window,
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

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
	SDL_DestroyWindow(Window);
	SDL_DestroyRenderer(Renderer);
	SDL_Quit();
}

void Game::ProcessInput()
{
	SDL_Event Event;
	while (SDL_PollEvent(&Event))
	{
		switch (Event.type)
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
	PaddleDir = 0;
	if (state[SDL_SCANCODE_W])
	{
		PaddleDir -= 1;
	}
	if (state[SDL_SCANCODE_S])
	{
		PaddleDir += 1;
	}
}

void Game::UpdateGame()
{
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), TicksCount + 16));// �����Ӱ� �����ӻ����� �ð��� 16ms�� �����ϱ� ���� �ڵ� 1000/60 = 16 : 1000ms(1��)���� 60���� �����ִ� 1��� ��� 16ms�ҿ�ʿ� ���� 16ms�� �����ֱ� ����

	float Deltatime = (SDL_GetTicks() - TicksCount) / 1000.0f;

	if (Deltatime > 0.05f) // �극��ũŸ���� �ɸ� ��ŸŸ���� �ʹ� ������� �Ҷ� ����. ���� �����ϱ� ���ؼ� ��ŸŸ���� �����ϰ� ������ٸ� 0.05�� ���������� �̷����ϸ� �е��̳� ���� �ʹ� ���� �̵��ϴ������� �����ش�.
	{
		Deltatime = 0.05f;
	}
	TicksCount = SDL_GetTicks();

	if (PaddleDir != 0)
	{
		PaddlePos.y += PaddleDir * 300.0f * Deltatime;
		if (PaddlePos.y < (paddleH / 2.0f + thickness))
		{
			PaddlePos.y = paddleH / 2.0f + thickness;
		}
		else if (PaddlePos.y > (768 - paddleH / 2.0f - thickness))
		{
			PaddlePos.y = 768 -paddleH / 2.0f-thickness;
		}
	}

	BallPos.x += BallVel.x*Deltatime;
	BallPos.y += BallVel.y*Deltatime;

	float diff = PaddlePos.y - BallPos.y;
	diff = (diff > 0) ? diff : -diff;
	if (diff < paddleH / 2.0f && // �е��� �ݴ� �κ��� ���̿� ��ġ�ϰ�
		BallPos.x <= 25.0f &&BallPos.x >= 20.0f && //�е��� �ݴ� �κб��� x��ǥ�� �����ϰ�
		BallVel.x<0.0f) // ���� ������ �е�������� ����ö�
	{
		BallVel.x *= -1.0f;
	}
	else if (BallPos.x <= 0)
	{
		bIsRunning = false;
	}
	else if (BallPos.x >= (1024.0f - thickness) && BallVel.x > 0.0f)
	{
		BallVel.x *= -1.0f;
	}

	if (BallPos.y <= thickness && BallVel.y < 0.0f)
	{
		BallVel.y *= -1.0f;
	}
	else if (BallPos.y > (768 - thickness) && BallVel.y > 0.0f)
	{
		BallVel.y *= -1.0f;
	}

}

void Game::GenerateOutput()
{
	SDL_SetRenderDrawColor(
		Renderer,
		0,
		0,
		255,
		255
	);
	SDL_RenderClear(Renderer);
	//���׸��� �κ�
	SDL_SetRenderDrawColor(Renderer, 255, 255, 255, 255);
	SDL_Rect Wall{ 0,0,1024,thickness };
	SDL_RenderFillRect(Renderer, &Wall);
	Wall.y = 768 - thickness;
	SDL_RenderFillRect(Renderer, &Wall);
	Wall.x = 1024 - thickness;
	Wall.y = 0;
	Wall.h = 1024;
	Wall.w = thickness;
	SDL_RenderFillRect(Renderer, &Wall);
	//
	//���׸��� �κ�
	SDL_Rect Ball
	{
	static_cast<int>(BallPos.x - thickness / 2),
	static_cast<int>(BallPos.y - thickness / 2),
	thickness,
	thickness
	};
	SDL_RenderFillRect(Renderer, &Ball);
	//
	//�е�׸��� �κ�
	SDL_Rect Paddle
	{
	static_cast<int>(PaddlePos.x),
	static_cast<int>(PaddlePos.y - paddleH/2),
	thickness,
	static_cast<int>(paddleH)
	};
	SDL_RenderFillRect(Renderer, &Paddle);
	//
	SDL_RenderPresent(Renderer);
}
