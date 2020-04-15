#pragma once
#include <vector>
#include "Math.h"
#include <cstdint>

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
	void UpdateComponents(float deltatime);
	virtual void UpdateActor(float deltatime);

	void ProcessInput(const uint8_t* keyState);
	virtual void ActorInput(const uint8_t* KeyState);

	const Vector2& GetPosition() const { return mPosition; }
	void SetPosition(const Vector2& pos) { mPosition = pos; }
	float GetScale() const { return mScale;}
	void SetScale(float scale) { mScale = scale; }
	float GetRotation() { return mRotation; }
	void SetRotation(float rotation) { mRotation = rotation; }

	Vector2 GeForward() const { return Vector2(Math::Cos(mRotation), -Math::Sin(mRotation)); }
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

