#pragma once
#include "Actor.h"
class Laser :
	public Actor
{
public:
	Laser(class Game* game);
	void UpdateActor(float deltatime) override;

private:
	class CircleComponent*  mCircle;
	float mDeathTimer;
};

