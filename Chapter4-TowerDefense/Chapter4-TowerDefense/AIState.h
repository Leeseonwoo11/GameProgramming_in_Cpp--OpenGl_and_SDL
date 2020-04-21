#pragma once
class AIState
{
public:
	AIState(class AIComponent* owner) :mOwner(owner) {}
	virtual void Update(float deltatime) = 0;// 순수가상함수 다른데서 재정의해서 사용함
	virtual void OnEnter() = 0; // 순수가상함수
	virtual void OnExit() = 0; // 순수가상함수

	virtual const char* GetName() const = 0;// 순수가상함수

protected:
	class AIComponent* mOwner;
};

class AIPatrol :public AIState
{
public:
	AIPatrol(class AIComponent* owner) : AIState(owner) {}

	void Update(float deltatime) override;
	void OnEnter() override;
	void OnExit() override;

	const char* GetName() const override
	{
		return "Patrol";
	}

};

class AIDeath : public AIState
{
public:
	AIDeath(class AIComponent* owner) : AIState(owner) {}
	
	void Update(float deltatime) override;
	void OnEnter() override;
	void OnExit() override;

	const char* GetName() const override
	{
		return "Death";
	}

};

class AIAttack : public AIState
{
public:
	AIAttack(class AIComponent* owner) : AIState(owner) {}

	void Update(float deltatime) override;
	void OnEnter() override;
	void OnExit() override;

	const char* GetName() const override
	{
		return "Attack";
	}

};