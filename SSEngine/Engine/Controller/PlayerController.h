#pragma once

#include "IController.h"

//#define BASIC_MOVEMENT
#define PHYSICS_MOVEMENT

class PlayerController : public IController
{
public:
	explicit FORCEINLINE PlayerController(const WeakPtr<Pawn> &i_pawn);

	virtual ~PlayerController();

	virtual void SetPawn(const WeakPtr<Pawn> &i_pawn) override { pawn_ = i_pawn; }
	virtual WeakPtr<Pawn> GetPawn() const { return pawn_; }
	virtual bool IsValid() const override { return pawn_; }
	
	virtual void UpdateController() override;

private:
	void handleBasicMoveFromUserInput() const;
	void handlePhysicsMoveFromUserInput() const;

private:
	// Controller can not be copy or assign
	FORCEINLINE PlayerController(const PlayerController &i_other) {}
	FORCEINLINE PlayerController& operator=(const PlayerController &i_other) {}

	WeakPtr<Pawn> pawn_;
};




// implement forceinline

FORCEINLINE PlayerController::PlayerController(const WeakPtr<Pawn> &i_pawn)
	: pawn_(i_pawn)
{
}


