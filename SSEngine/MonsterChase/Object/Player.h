#pragma once

#include "Engine\Core\Math\Vector3.h"
#include "Engine\Controller\PlayerController.h"
#include <utility>

#define DEFAULT_CHAR_LENGTH 50

class Player
{
public:
	FORCEINLINE Player();
	explicit FORCEINLINE Player(const char* i_Name);
	FORCEINLINE Player(const char* i_Name, const Vector3 &i_Position);
	
	FORCEINLINE Player(const Player & i_Player);
	FORCEINLINE Player(Player && i_Player);
	FORCEINLINE Player & operator= (const Player &i_Player);
	FORCEINLINE Player & operator= (Player &&i_Player);
	FORCEINLINE ~Player();

	FORCEINLINE const char* GetName() const { return name_; }
	FORCEINLINE PlayerController *GetController() const { return playerController_; }
	FORCEINLINE Vector3 GetPosition() const { return playerController_->GetPosition(); };

	FORCEINLINE void Move() const;
	FORCEINLINE void Update() const;

private:
	char *name_;
	PlayerController *playerController_;
};










// implement forceinline

FORCEINLINE Player::Player() 
	: name_(_strdup("Player")), playerController_(new PlayerController()) 
{
}

FORCEINLINE Player::Player(const char* i_Name)
	: name_(_strdup(i_Name ? i_Name : "Player")), playerController_(new PlayerController()) 
{
}

FORCEINLINE Player::Player(const char* i_Name, const Vector3 &i_Position) 
	: name_(_strdup(i_Name ? i_Name : "Player")), playerController_(new PlayerController(i_Position)) 
{
}

FORCEINLINE Player::Player(const Player & i_Player) 
	: name_(_strdup(i_Player.name_ ? i_Player.name_ : "Player")), playerController_(new PlayerController(*i_Player.playerController_)) 
{
}

FORCEINLINE Player::Player(Player && i_Player) 
	: name_(i_Player.name_), playerController_(i_Player.playerController_)
{
	i_Player.name_ = nullptr;
	i_Player.playerController_ = nullptr;
}

FORCEINLINE Player & Player::operator= (const Player &i_Player)
{
	if (name_)
		free(name_);
	name_ = _strdup(i_Player.name_ ? i_Player.name_ : "Player");
	if (playerController_)
		delete playerController_;
	playerController_ = new PlayerController(*i_Player.playerController_);
}

FORCEINLINE Player & Player::operator= (Player &&i_Player)
{
	std::swap(name_, i_Player.name_);
	std::swap(playerController_, i_Player.playerController_);
}

FORCEINLINE Player::~Player()
{
	if (playerController_)
		delete playerController_;
	if (name_)
		free(name_);
}

FORCEINLINE void Player::Move() const
{
	playerController_->GetMovementDirectionFromUserInput();
}

FORCEINLINE void Player::Update() const
{
	playerController_->UpdateGameObject();
}
