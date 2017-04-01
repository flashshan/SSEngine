#pragma once

#include "Engine\Controller\IController.h"


//#include <utility>
class FocusMoveController : public IController
{
public:
	explicit FORCEINLINE FocusMoveController(const StrongPtr<Pawn>& i_pawn, const StrongPtr<Pawn> &i_focusPawn);

	virtual ~FocusMoveController();

	virtual void SetPawn(const StrongPtr<Pawn>& i_pawn) override { pawn_ = i_pawn; }
	virtual bool IsValid() const override { return pawn_; }

	virtual void UpdateController() override;

private:
	void focusMovePawn();

private:
	// Controller can not be copy or assign
	FORCEINLINE FocusMoveController(const FocusMoveController &i_other) {}
	FORCEINLINE FocusMoveController& operator=(const FocusMoveController &i_other) {}

	WeakPtr<Pawn> pawn_;
	WeakPtr<Pawn> focusPawn_;
};









// implement forceinline
FORCEINLINE FocusMoveController::FocusMoveController(const StrongPtr<Pawn>& i_pawn, const StrongPtr<Pawn> &i_focusPawn)
	: pawn_(i_pawn), focusPawn_(i_focusPawn)
{
}