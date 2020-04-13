#pragma once
#include <vector>
#include "Math.h"

class Actor
{
public:
	enum State
	{
		EActive,
		EPaused,
		EDead
	};

	Actor(class Game* game);
	virtual ~Actor();

	void Update(float deltatime);
	void UpdateComponent(float deltatime);
	virtual void UpdateActor(float deltatime);

	const Vector2& GetPosition() const { return mPosition; }
	void SetPosition(const Vector2& pos) { mPosition = pos; }
	float GetScale() { return mScale;}
	void SetScale(float scale) { mScale = scale; }
	float GetRotation() { return mRotation; }
	void SetRotation(float rot) { mRotation = rot; }

	State GetState() { return mState; }
	void SetState(State state) { mState = state; }

	class Game* GetGame() { return mGame; }

	void AddComponent(class Component* component);
	void RemoveComponent(class Component* component);

private:
	State mState;
	Vector2 mPosition;
	float mScale;
	float mRotation;

	std::vector<class Component*> mComponents;
	class Game* mGame;

};
