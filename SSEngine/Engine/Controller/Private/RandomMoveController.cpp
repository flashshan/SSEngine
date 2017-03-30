#include "Controller\RandomMoveController.h"
#include "Object\Pawn.h"

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
	const float speed = 3.0f;
	(*pawn_).SetActorVelocity(Vector3(Vector2::RandomNormal() * speed));
}