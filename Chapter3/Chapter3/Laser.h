#pragma once
#include "Actor.h"
class Laser :
	public Actor
{
public:
	Laser(class Game* game);
	~Laser();
	void UpdateActor(float deltatime) override;
	void SetDeathTimer(float time) { mDeathTimer = time; }
private:
	class CircleComponent*  mCircle;
	class SpriteComponent* sc;
	class MoveComponent* mc;
	float mDeathTimer;
};

