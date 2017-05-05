#pragma once

#include "Engine/Controller/IController.h"


class ConstantMoveController : public IController
{
public:
	explicit FORCEINLINE ConstantMoveController(const WeakPtr<Pawn> &i_pawn);

	virtual ~ConstantMoveController();

	virtual void SetPawn(const WeakPtr<Pawn> &i_pawn) override { pawn_ = i_pawn; }
	virtual WeakPtr<Pawn> GetControlledPawn() const { return pawn_; }
	virtual bool IsValid() const override { return pawn_; }

	virtual void UpdateController() override;

private:
	void constantMovePawn();

private:
	// No copy allowed
	FORCEINLINE ConstantMoveController(const ConstantMoveController & i_other) {}
	FORCEINLINE ConstantMoveController& operator=(const ConstantMoveController &i_other) {}

	WeakPtr<Pawn> pawn_;
};









// implement forceinline

FORCEINLINE ConstantMoveController::ConstantMoveController(const WeakPtr<Pawn> &i_pawn)
	: pawn_(i_pawn)
{
}