#include "Ship.h"
#include "AnimSpriteComponent.h"
#include "Game.h"

Ship::Ship(Game * game)
	:Actor(game),
	mRightSpeed(0.0f),
	mDownSpeed(0.0f)
{
	Idle = new AnimSpriteComponent(this);
	std::vector<SDL_Texture* > Idleanims;
	Idleanims = {
		game->GetTexture("Assets/Character16.png")
	};
	Idle->SetAnsimTextures(Idleanims);
	Idle->SetRunActive(true);
	
	Walk = new AnimSpriteComponent(this);
	std::vector<SDL_Texture*> Walkanims;
	Walkanims = {
		game->GetTexture("Assets/Character01.png"),
		game->GetTexture("Assets/Character02.png"),
		game->GetTexture("Assets/Character03.png"),
		game->GetTexture("Assets/Character04.png"),
		game->GetTexture("Assets/Character05.png"),
		game->GetTexture("Assets/Character06.png")
	};
	Walk->SetAnsimTextures(Walkanims);
	Walk->SetRunActive(false);
	
	Punch = new AnimSpriteComponent(this);
	std::vector<SDL_Texture*>Punchanims = {
	game->GetTexture("Assets/Character16.png"),
	game->GetTexture("Assets/Character17.png"),
	game->GetTexture("Assets/Character18.png")
	};
	Punch->SetAnsimTextures(Punchanims);
	Punch->SetRunActive(false);
}


void Ship::UpdateActor(float deltatime)
{
	Actor::UpdateActor(deltatime);
	Vector2 pos = GetPosition();
	pos.x += mRightSpeed * deltatime;
	pos.y += mDownSpeed * deltatime;
	if (pos.x < 25.0f)
	{
		pos.x = 25.0f;
	}
	else if (pos.x > 500.0f)
	{
		pos.x = 500.0f;
	}
	if (pos.y < 25.0f)
	{
		pos.y = 25.0f;
	}
	else if (pos.y > 743.0f)
	{
		pos.y = 743.0f;
	}
	SetPosition(pos);
}

void Ship::ProcessKeyboard(const uint8_t * state)
{
	mRightSpeed = 0.0f;
	mDownSpeed = 0.0f;

	if (state[SDL_SCANCODE_SPACE] && !bIsRun)
	{
		Punch->SetRunActive(true);
		Punch->SetAnimFPS(12.0f);
		Walk->SetRunActive(false);
		Idle->SetRunActive(false);
		bIsAttack = true;
		return;
	}

	if (state[SDL_SCANCODE_A])
	{
		mRightSpeed -= 250.0f;
		Walk->SetRunActive(true);
		Idle->SetRunActive(false);
		bIsRun = true;
		return;
	}
	if (state[SDL_SCANCODE_D])
	{
		mRightSpeed += 250.0f;
		Walk->SetRunActive(true);
		Idle->SetRunActive(false);
		bIsRun = true;
		return;
	}
	else
	{	
		Walk->SetRunActive(false);
		Idle->SetRunActive(true);
		Punch->SetRunActive(false);
		bIsRun = false;
		bIsAttack = false;
		return;
	}
}
