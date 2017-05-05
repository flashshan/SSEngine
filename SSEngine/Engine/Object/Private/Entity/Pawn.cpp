#include "Object\Entity\Pawn.h"

Pawn::~Pawn()
{
}

void Pawn::EarlyUpdate()
{
	Actor::EarlyUpdate();
}

void Pawn::Update()
{
	Actor::Update();
}

void Pawn::LateUpdate()
{
	Actor::LateUpdate();
}
