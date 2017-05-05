#include "Controller\RandomMoveController.h"
#include "Object\Entity\Pawn.h"

RandomMoveController::~RandomMoveController()
{
}

void RandomMoveController::UpdateController()
{
	if (pawn_)
	{
		randomMovePawn();
	}
}

void RandomMoveController::randomMovePawn()
{
	const float speed = 20.0f;
	pawn_->SetActorVelocity(Vector3(Vector2::RandomNormal() * speed));
}