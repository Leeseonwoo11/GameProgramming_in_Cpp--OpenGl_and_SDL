#include "Ship.h"
#include "SpriteComponent.h"
#include "InputComponent.h"
#include "Game.h"
#include "Laser.h"
#include "CircleComponent.h"
#include "Asteroid.h"
#include "LaserPool.h"

#include <iostream>


Ship::Ship(Game * game)
	:Actor(game)
	, mLaserCooldown(0.0f)
{
	SpriteComponent* sc = new SpriteComponent(this);
	sc->SetTexture(GetGame()->GetTexture("Assets/Ship.png"));

	InputComponent* ic = new InputComponent(this);
	ic->SetForwardKey(SDL_SCANCODE_W);
	ic->SetBackKey(SDL_SCANCODE_S);
	ic->SetClockwiseKey(SDL_SCANCODE_A);
	ic->SetCounterClockwiseKey(SDL_SCANCODE_D);
	ic->SetMaxForwardSpeed(300.0f);
	ic->SetMaxAngularSpeed(Math::TwoPi);

	Circle = new CircleComponent(this);
	Circle->SetRadius(40.0f);

	mLaserPool = new LaserPool(game,40);
}


void Ship::UpdateActor(float deltatime)
{
	mLaserCooldown -= deltatime;
	ShipCoolTime -= deltatime;
	for (auto ast : GetGame()->GetAsteroids())
	{
		if (Intersect(*Circle, *ast->GetCircle()))
		{
			ShipCoolTime = 2.0f; // 총돌시 쿨타임을 3초로 셋팅한다.
		}
	}
	if (ShipCoolTime > 0.0f) // 쿨타임이 도는동안
	{
		SetPosition(Vector2(-1000, -1000)); // 우주선의 위치를 화면 밖으로 보내버린다.
		SetRotation(0.0f); // 회전값을 초기화 한다.
		flag = true; // 우주선이 제자리로 돌아오기 위한 플래그를 초기화한다.
		mLaserCooldown = 0.1; // 쿨타임이 도는동안 레이저 발사를 막기 위해서  0.1로 초기화 해준다.	
	}
	else //  쿨타임이 돌아오면
	{
		if (flag) // 제자리로 돌아오는 플래그가 활성화 되어있다면
		{
			SetPosition(Vector2(1024.0f / 2, 768.0f / 2)); // 우주선 포지션을 중앙으로 옮겨준다.
			SetRotation(Math::Pi/2); //  회전값도 초기화 해준다.
			flag = false; // 제자리로 돌아왔으니 플래그를 false로 초기화 해준다.
		}
	}


}

void Ship::ActorInput(const uint8_t * keystate)
{
	if (keystate[SDL_SCANCODE_SPACE] && mLaserCooldown <= 0.0f)
	{
		Laser* temp = mLaserPool->GetLaser();
		if (temp != nullptr)
		{
			temp->SetState(Actor::EActive);
			temp->SetPosition(GetPosition());
			temp->SetRotation(GetRotation());
		}
	}
}
