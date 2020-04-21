#pragma once
#include "Actor.h"
class Tower :
	public Actor
{
public:
	Tower(class Game* game);
	void UpdateActor(float deltatime) override;
private:
	class MoveComponent* mMove;
	float mNextAttack;
	const float AttackTime = 2.5f;
	const float AttackRange = 100.0f;
};

