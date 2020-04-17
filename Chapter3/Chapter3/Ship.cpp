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
			ShipCoolTime = 2.0f; // �ѵ��� ��Ÿ���� 3�ʷ� �����Ѵ�.
		}
	}
	if (ShipCoolTime > 0.0f) // ��Ÿ���� ���µ���
	{
		SetPosition(Vector2(-1000, -1000)); // ���ּ��� ��ġ�� ȭ�� ������ ����������.
		SetRotation(0.0f); // ȸ������ �ʱ�ȭ �Ѵ�.
		flag = true; // ���ּ��� ���ڸ��� ���ƿ��� ���� �÷��׸� �ʱ�ȭ�Ѵ�.
		mLaserCooldown = 0.1; // ��Ÿ���� ���µ��� ������ �߻縦 ���� ���ؼ�  0.1�� �ʱ�ȭ ���ش�.	
	}
	else //  ��Ÿ���� ���ƿ���
	{
		if (flag) // ���ڸ��� ���ƿ��� �÷��װ� Ȱ��ȭ �Ǿ��ִٸ�
		{
			SetPosition(Vector2(1024.0f / 2, 768.0f / 2)); // ���ּ� �������� �߾����� �Ű��ش�.
			SetRotation(Math::Pi/2); //  ȸ������ �ʱ�ȭ ���ش�.
			flag = false; // ���ڸ��� ���ƿ����� �÷��׸� false�� �ʱ�ȭ ���ش�.
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
