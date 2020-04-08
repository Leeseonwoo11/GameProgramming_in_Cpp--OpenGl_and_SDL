#pragma once

#include "pch.h"
#include "Game.h"
#include <iostream>


const int thickness = 15; // ���� �β�	
const float paddleH = 200.0f; // �е��� ����
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
	PaddlePos.x = 10.0f; //  1p �е� ��ġ
	PaddlePos.y = 768.0f / 2;

	Ball a;
	a.BallPos.x = 1024.0f / 2; // �� ���� ��ġ
	a.BallPos.y = 768 / 2;
	a.BallVel.x = -200.0f; //  �� ó�� ����
	a.BallVel.y = 235.0f;
	Ballvec.push_back(a);



	PaddlePos2.x = 1024.0f - 10.0f; // 2p�� �е� ��ġ
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
			b.BallPos.x = 1024.0f / 2; // �� ���� ��ġ
			b.BallPos.y = 768 / 2;
			b.BallVel.x = -200.0f*num; //  �� ó�� ����
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
			b.BallPos.x = 1024.0f / 2; // �� ���� ��ġ
			b.BallPos.y = 768 / 2;
			b.BallVel.x = -200.0f*num; //  �� ó�� ����
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
		diff = (diff > 0.0f) ? diff : -diff; // �е�� ���� y��ǥ ���̸� ���밪���� �ٲ�
		if (diff <= paddleH / 2.0f &&   // �е��� ���� �ݰ濡 �ְ�
			Ballvec[i].BallPos.x <= 25.0f && Ballvec[i].BallPos.x >= 15.0f && // ���� x��ǥ�� �е��� �β��� ������ʰ� 
			Ballvec[i].BallVel.x < 0.0f) // ���� ������ <-(����)�������� ������»��� ���
		{
			Ballvec[i].BallVel.x *= -1.0f; // ����x ������ �ݴ�� �����ش� 
		}
		else if (Ballvec[i].BallPos.x <= 0.0f)
		{
			IsRunning = false; // ���� �ٱ����� �������� ���ӿ���
		}

		float diff2 = PaddlePos2.y - Ballvec[i].BallPos.y;
		diff2 = (diff2 > 0.0f) ? diff2 : -diff2; // �е�� ���� y��ǥ ���̸� ���밪���� �ٲ�
		if (diff2 <= paddleH / 2.0f &&   // �е��� ���� �ݰ濡 �ְ�
			Ballvec[i].BallPos.x >= 1024 - 25.0f && Ballvec[i].BallPos.x <= 1024 - 20.0f && // ���� x��ǥ�� �е��� �β��� ������ʰ� 
			Ballvec[i].BallVel.x > 0.0f) // ���� ������ ->(������)�������� ������»��� ���
		{
			Ballvec[i].BallVel.x *= -1.0f; // ����x ������ �ݴ�� �����ش� 
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
	SDL_SetRenderDrawColor(Renderer, 0, 0, 255, 255); // ���� ����� �׸���.

	SDL_RenderClear(Renderer); // ������ Ŭ����

	SDL_SetRenderDrawColor(Renderer, 255, 255, 255, 255); //���� �׸��� ���� �������

	SDL_Rect wall //���� �׸���
	{
	0,  // ������ x��
	0, //  ������ y��
	1024,  // �ʺ�
	thickness // ����
	};
	SDL_RenderFillRect(Renderer, &wall); // wall�� ������ õ�� ������ �̸� ������ ������ ä���ش�.
	wall.y = 768.0f - thickness; //������ y ���� �Ʒ� �������� �����Ѵ�.
	SDL_RenderFillRect(Renderer, &wall); // ������ �Ʒ������� �׷��ش�.


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
		SDL_RenderFillRect(Renderer, &Ball); // �� �׸���
	}
	SDL_RenderPresent(Renderer); // �չ��ۿ� �޹��۸� �����Ѵ�.

}
