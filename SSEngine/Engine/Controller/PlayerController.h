#pragma once

#include "IController.h"

//#define BASIC_MOVEMENT
#define PHYSICS_MOVEMENT

class PlayerController : public IController
{
public:
	explicit FORCEINLINE PlayerController(const StrongPtr<Pawn>& i_pawn);

	virtual ~PlayerController();

	virtual void SetPawn(const StrongPtr<Pawn>& i_pawn) override { pawn_ = i_pawn; }
	virtual bool IsValid() const override { return pawn_; }
	
	virtual void UpdateController() override;

private:
	void handleBasicMoveFromUserInput();
	void handlePhysicsMoveFromUserInput();

private:
	// Controller can not be copy or assign
	FORCEINLINE PlayerController(const PlayerController &i_PlayerController) {}

	WeakPtr<Pawn> pawn_;
};




// implement forceinline

FORCEINLINE PlayerController::PlayerController(const StrongPtr<Pawn>& i_pawn)
	: pawn_(i_pawn)
{
}


