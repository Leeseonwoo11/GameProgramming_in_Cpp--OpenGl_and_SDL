#pragma once
#include "Actor.h"
class Ship :
	public Actor
{
public:
	Ship(class Game* game);
	void UpdateActor(float deltatime) override;
	void ProcessKeyboard(const uint8_t* state);
	float GetRightSpeed()const { return mRightSpeed; }
	float GetDownSpeed()const { return mDownSpeed; }
private:
	float mRightSpeed;
	float mDownSpeed;
	class AnimSpriteComponent* Idle;
	class AnimSpriteComponent* Walk;
	class AnimSpriteComponent* Punch;
	bool bIsRun = false;
	bool bIsAttack = false;

};
