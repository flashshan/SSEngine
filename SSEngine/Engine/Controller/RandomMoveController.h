#pragma once

#include "Engine/Controller/IController.h"


class RandomMoveController : public IController
{
public:
	explicit FORCEINLINE RandomMoveController(const StrongPtr<Pawn> &i_pawn);

	virtual ~RandomMoveController();

	virtual void SetPawn(const StrongPtr<Pawn>& i_pawn) override { pawn_ = i_pawn; }
	virtual bool IsValid() const override { return pawn_; }

	virtual void UpdateController() override;

private:
	void randomMovePawn();

private:
	// No copy allowed
	FORCEINLINE RandomMoveController(const RandomMoveController & i_other) {}

	WeakPtr<Pawn> pawn_;
};









// implement forceinline

FORCEINLINE RandomMoveController::RandomMoveController(const StrongPtr<Pawn> &i_pawn)
	: pawn_(i_pawn)
{
}