#include "Controller\FocusMoveController.h"
#include "Object\Pawn.h"

#include "Manager\RealTimeManager.h"

FocusMoveController::~FocusMoveController()
{
}


void FocusMoveController::UpdateController()
{
	if (pawn_)
	{
		focusMovePawn();
	}
}

void FocusMoveController::focusMovePawn()
{
	const float maxSpeed = 50.0f;
	const float accelerate = 5.0f;

	if (pawn_ && focusPawn_)
	{
		Vector3 velocity = pawn_->GetActorVelocity() + ((*focusPawn_).GetActorLocation() - (*pawn_).GetActorLocation()).Normalize() * accelerate * RealTimeManager::GetInstance()->GetLastFrameTimeS();
		if (velocity.LengthSquare() > maxSpeed * maxSpeed)
		{
			velocity = velocity.Normalize() * maxSpeed;
		}
		pawn_->SetActorVelocity(velocity);
	}
}
