#include "Laser.h"
#include "SpriteComponent.h"
#include "MoveComponent.h"
#include "Game.h"
#include "CircleComponent.h"
#include "Asteroid.h"

Laser::Laser(Game * game)
	:Actor(game),
	mDeathTimer(1.0f)
{
	sc = new SpriteComponent(this);
	sc->SetTexture(game->GetTexture("Assets/Laser.png"));
	mc = new MoveComponent(this);
	mc->SetForwardSpeed(880.0f);

	mCircle = new CircleComponent(this);
	mCircle->SetRadius(11.0f);

}

Laser::~Laser()
{

}

void Laser::UpdateActor(float deltatime)
{
	mDeathTimer -= deltatime;
	if (mDeathTimer <= 0.0f)
	{
		SetState(EPaused);
	}
	else
	{
		for (auto ast : GetGame()->GetAsteroids())
		{
			if (Intersect(*mCircle, *(ast->GetCircle())))
			{
				SetState(EPaused); // 상태 정지상태
				ast->SetState(EDead); // 마찬가지로 소행성도 상태 죽음으로 바꿔 삭제한다.
				break;
			}
		}
	}
	if (GetState() == Actor::EPaused)
	{
		mc->SetForwardSpeed(0.0f);
		SetPosition(Vector2(-100,-100));
	}
	else if (GetState() == Actor::EActive)
	{
		mc->SetForwardSpeed(880.0f);
	}

}
