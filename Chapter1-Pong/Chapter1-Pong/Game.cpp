#pragma once

#include "pch.h"
#include "Game.h"
#include <iostream>


const int thickness = 15; // 벽의 두께	
const float paddleH = 200.0f; // 패들의 높이
const float PaddleSpeed = 500.0f;
const int BallSpeed = 1.0f;
float num = -1;
Game::Game() :Window(nullptr)
, Renderer(nullptr)
, TickCount(0)
, IsRunning(true)
{

}

bool Game::Initialize()
{
	Window = SDL_CreateWindow("CreateWindow",
		10,
		10,
		1024,
		768,
		00);
	if (Window == nullptr)
	{
		SDL_Log("Window is nullptr Errcode is %s", SDL_GetError());
		return false;
	}
	Renderer = SDL_CreateRenderer(Window, -1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (Renderer == nullptr)
	{
		SDL_Log("Fail create Renderer %s", SDL_GetError());
		return false;
	}
	PaddlePos.x = 10.0f; //  1p 패들 위치
	PaddlePos.y = 768.0f / 2;

	Ball a;
	a.BallPos.x = 1024.0f / 2; // 공 시작 위치
	a.BallPos.y = 768 / 2;
	a.BallVel.x = -200.0f; //  공 처음 방향
	a.BallVel.y = 235.0f;
	Ballvec.push_back(a);



	PaddlePos2.x = 1024.0f - 10.0f; // 2p의 패들 위치
	PaddlePos2.y = 768.0f / 2;

	return true;
}

void Game::Runloop()
{
	while (IsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}

}

void Game::Shutdown()
{
	SDL_DestroyRenderer(Renderer);
	SDL_DestroyWindow(Window);
	SDL_Quit();
}

void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			IsRunning = false;
			break;
		}
	}

	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE])
	{
		IsRunning = false;
	}
	paddleDir = 0;
	paddleDir2 = 0;
	if (state[SDL_SCANCODE_W])
	{
		paddleDir -= 1;
	}
	if (state[SDL_SCANCODE_S])
	{
		paddleDir += 1;
	}
	if (state[SDL_SCANCODE_UP])
	{
		paddleDir2 -= 1;
	}
	if (state[SDL_SCANCODE_DOWN])
	{
		paddleDir2 += 1;
	}
	if (state[SDL_SCANCODE_SPACE])
	{
		if (flag)
		{
			Ball b;
			b.BallPos.x = 1024.0f / 2; // 공 시작 위치
			b.BallPos.y = 768 / 2;
			b.BallVel.x = -200.0f*num; //  공 처음 방향
			b.BallVel.y = 235.0f*num;
			Ballvec.push_back(b);
			flag = false;
			num *= -1;
		}
	}
	if (state[SDL_SCANCODE_RSHIFT])
	{
		if (!flag)
		{
			Ball b;
			b.BallPos.x = 1024.0f / 2; // 공 시작 위치
			b.BallPos.y = 768 / 2;
			b.BallVel.x = -200.0f*num; //  공 처음 방향
			b.BallVel.y = 235.0f*num;
			Ballvec.push_back(b);
			flag = true;
			num *= -1;
		}
	}

}

void Game::UpdateGame()
{
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), TickCount + 16));

	float deltatime = (SDL_GetTicks() - TickCount) / 1000.0f;

	if (deltatime > 0.05f)
		deltatime = 0.05f;
	TickCount = SDL_GetTicks();
	if (paddleDir != 0)
	{
		PaddlePos.y += paddleDir * PaddleSpeed * deltatime;
		if (PaddlePos.y < (paddleH / 2.0f + thickness))
		{
			PaddlePos.y = (paddleH / 2.0f + thickness);
		}
		else if (PaddlePos.y > (768.0f - paddleH / 2.0f - thickness))
		{
			PaddlePos.y = (768.0f - paddleH / 2.0f - thickness);
		}
	}
	if (paddleDir2 != 0)
	{
		PaddlePos2.y += paddleDir2 * PaddleSpeed * deltatime;
		if (PaddlePos2.y < (paddleH / 2.0f + thickness))
		{
			PaddlePos2.y = (paddleH / 2.0f + thickness);
		}
		else if (PaddlePos2.y > (768.0f - paddleH / 2.0f - thickness))
		{
			PaddlePos2.y = (768.0f - paddleH / 2.0f - thickness);
		}
	}
	for (int i = 0; i < Ballvec.size(); i++)
	{
		Ballvec[i].BallPos.x += BallSpeed * Ballvec[i].BallVel.x*deltatime;
		Ballvec[i].BallPos.y += BallSpeed * Ballvec[i].BallVel.y*deltatime;

		float diff = PaddlePos.y - Ballvec[i].BallPos.y;
		diff = (diff > 0.0f) ? diff : -diff; // 패들과 공의 y좌표 차이를 절대값으로 바꿈
		if (diff <= paddleH / 2.0f &&   // 패들의 높이 반경에 있고
			Ballvec[i].BallPos.x <= 25.0f && Ballvec[i].BallPos.x >= 15.0f && // 공의 x좌표가 패들의 두께를 벗어나지않고 
			Ballvec[i].BallVel.x < 0.0f) // 공의 방향이 <-(왼쪽)방향으로 날라오는상태 라면
		{
			Ballvec[i].BallVel.x *= -1.0f; // 공의x 방향을 반대로 돌려준다 
		}
		else if (Ballvec[i].BallPos.x <= 0.0f)
		{
			IsRunning = false; // 공이 바깥으로 빠져나감 게임오버
		}

		float diff2 = PaddlePos2.y - Ballvec[i].BallPos.y;
		diff2 = (diff2 > 0.0f) ? diff2 : -diff2; // 패들과 공의 y좌표 차이를 절대값으로 바꿈
		if (diff2 <= paddleH / 2.0f &&   // 패들의 높이 반경에 있고
			Ballvec[i].BallPos.x >= 1024 - 25.0f && Ballvec[i].BallPos.x <= 1024 - 20.0f && // 공의 x좌표가 패들의 두께를 벗어나지않고 
			Ballvec[i].BallVel.x > 0.0f) // 공의 방향이 ->(오른쪽)방향으로 날라오는상태 라면
		{
			Ballvec[i].BallVel.x *= -1.0f; // 공의x 방향을 반대로 돌려준다 
		}
		else if (Ballvec[i].BallPos.x > 1024.0f && Ballvec[i].BallVel.x > 0.0f)
		{
			IsRunning = false;
		}

		if (Ballvec[i].BallPos.y >= (768 - thickness) && Ballvec[i].BallVel.y > 0.0f)
		{
			Ballvec[i].BallVel.y *= -1;
		}
		else if (Ballvec[i].BallPos.y <= 0.0f + thickness && Ballvec[i].BallVel.y < 0.0f)
		{
			Ballvec[i].BallVel.y *= -1;
		}
	}

}

void Game::GenerateOutput()
{
	SDL_SetRenderDrawColor(Renderer, 0, 0, 255, 255); // 먼저 배경을 그린다.

	SDL_RenderClear(Renderer); // 랜더를 클리어

	SDL_SetRenderDrawColor(Renderer, 255, 255, 255, 255); //벽을 그리기 위한 색깔셋팅

	SDL_Rect wall //도형 그리기
	{
	0,  // 시작점 x축
	0, //  시작점 y축
	1024,  // 너비
	thickness // 높이
	};
	SDL_RenderFillRect(Renderer, &wall); // wall로 정해준 천장 벽면을 미리 설정한 색으로 채워준다.
	wall.y = 768.0f - thickness; //시작점 y 수정 아래 벽면으로 수정한다.
	SDL_RenderFillRect(Renderer, &wall); // 수정한 아래벽면을 그려준다.


	SDL_Rect Paddle
	{
		static_cast<int>(PaddlePos.x),
		static_cast<int>(PaddlePos.y - paddleH / 2),
		thickness,
		static_cast<int>(paddleH)
	};
	SDL_RenderFillRect(Renderer, &Paddle);

	SDL_Rect Paddle2
	{
		static_cast<int>(PaddlePos2.x),
		static_cast<int>(PaddlePos2.y - paddleH / 2),
		thickness,
		static_cast<int>(paddleH)
	};
	SDL_RenderFillRect(Renderer, &Paddle2);

	for (int i = 0; i < Ballvec.size(); i++)
	{
		SDL_Rect Ball
		{
			static_cast<int>(Ballvec[i].BallPos.x),
			static_cast<int>(Ballvec[i].BallPos.y),
			thickness,
			thickness
		};
		SDL_RenderFillRect(Renderer, &Ball); // 공 그리기
	}
	SDL_RenderPresent(Renderer); // 앞버퍼와 뒷버퍼를 스왑한다.

}
