#pragma once

#include "IGameObjectController.h"

#include "Object\GameObject.h"
#include "Core\Math\Vector3.h"

#include <utility>

class PlayerController : IController
{
public:
	FORCEINLINE PlayerController();
	explicit FORCEINLINE PlayerController(Vector3 i_Position);
	FORCEINLINE PlayerController(const PlayerController &i_PlayerController);
	FORCEINLINE PlayerController(PlayerController &&i_PlayerController);
	virtual ~PlayerController();

	FORCEINLINE PlayerController &operator=(const PlayerController &i_PlayerController);
	FORCEINLINE PlayerController &operator=(PlayerController &&i_PlayerController);

	virtual FORCEINLINE void SetPawn(StrongPtr<Pawn> &i_pawn) override { pawn_ = i_pawn; }
	virtual FORCEINLINE Pawn *GetPawn() const override;
	
	void GetMovementDirectionFromUserInput();

private:
	WeakPtr<Pawn> pawn_;
	Vector3 forceDirection_;
	Vector3 velocity_;
};




// implement forceinline

FORCEINLINE PlayerController::PlayerController()
	: gameObject_(new GameObject()), velocity_(0, 0, 0)
{
}

FORCEINLINE PlayerController::PlayerController(Vector3 i_Position)
	: gameObject_(new GameObject(i_Position)), velocity_(0, 0, 0)
{
}

FORCEINLINE PlayerController::PlayerController(const PlayerController &i_PlayerController)
	: gameObject_(new GameObject(i_PlayerController.GetPosition())), forceDirection_(i_PlayerController.forceDirection_), velocity_(i_PlayerController.velocity_)
{
}

FORCEINLINE PlayerController::PlayerController(PlayerController &&i_PlayerController)
	: gameObject_(i_PlayerController.GetGameObject()), forceDirection_(i_PlayerController.forceDirection_), velocity_(i_PlayerController.velocity_)
{
	i_PlayerController.gameObject_ = nullptr;
}

FORCEINLINE PlayerController &PlayerController::operator=(const PlayerController &i_PlayerController)
{
	gameObject_ = new GameObject(i_PlayerController.GetPosition());
	forceDirection_ = i_PlayerController.forceDirection_;
	velocity_ = i_PlayerController.velocity_;
	return *this;
}

FORCEINLINE PlayerController &PlayerController::operator=(PlayerController &&i_PlayerController)
{
	std::swap(gameObject_, i_PlayerController.gameObject_);
	std::swap(forceDirection_, i_PlayerController.forceDirection_);
	std::swap(velocity_, i_PlayerController.velocity_);
	return *this;
}
