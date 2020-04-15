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
	SpriteComponent* sc = new SpriteComponent(this);
	sc->SetTexture(game->GetTexture("Assets/Laser.png"));
	MoveComponent* mc = new MoveComponent(this);
	mc->SetForwardSpeed(880.0f);

	mCircle = new CircleComponent(this);
	mCircle->SetRadius(11.0f);
}

void Laser::UpdateActor(float deltatime)
{
	mDeathTimer -= deltatime;
	if (mDeathTimer <= 0.0f)
	{
		SetState(EDead);
	}
	else
	{
		for (auto ast : GetGame()->GetAsteroids())
		{
			if (Intersect(*mCircle, *(ast->GetCircle())))
			{
				SetState(EDead); // 직접 없앨수도 있지만 안전하게 상태를 바꿔서 레이져 스스로 삭제되게 한다.
				ast->SetState(EDead); // 마찬가지로 상태 죽음으로 바꿔 삭제한다.
				break;
			}
		}
	}

}
