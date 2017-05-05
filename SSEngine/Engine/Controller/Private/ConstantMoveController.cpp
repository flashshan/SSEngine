#include "Controller\ConstantMoveController.h"
#include "Object\Entity\Pawn.h"

ConstantMoveController::~ConstantMoveController()
{
}

void ConstantMoveController::UpdateController()
{
	if (pawn_)
	{
		constantMovePawn();
	}
}

void ConstantMoveController::constantMovePawn()
{
	const float speed = 20.0f;
	pawn_->SetActorVelocity(Vector3(Vector2::RandomNormal() * speed));
}