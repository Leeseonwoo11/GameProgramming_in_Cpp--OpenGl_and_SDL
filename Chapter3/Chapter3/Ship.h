#pragma once
#include "Actor.h"
class Ship :
	public Actor
{
public:
	Ship(class Game* game);

	void UpdateActor(float deltatime) override;
	void ActorInput(const uint8_t* keystate) override;

private:
	float mLaserCooldown;
	class CircleComponent* Circle;
	float ShipCoolTime = 0.0f;
	bool flag = false;

};

