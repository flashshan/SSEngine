#include "Controller\PlayerController.h"

#include "Manager\InputManager.h"
#include "Object/Pawn.h"

PlayerController::~PlayerController()
{
}

void PlayerController::UpdateController()
{
	if (pawn_)
	{
#ifdef BASIC_MOVEMENT
		handleBasicMoveFromUserInput();
#elif defined PHYSICS_MOVEMENT
		handlePhysicsMoveFromUserInput();
#endif
	}
}

void PlayerController::handleBasicMoveFromUserInput()
{
	const float defaultVelocity = 0.1f;
	Vector3 velocity(0.0f, 0.0f, 0.0f);

	if (InputManager::GetInstance()->GetState(static_cast<uint32>(Key::A)))
	{
		velocity += Vector3(-defaultVelocity, 0, 0);
	}
	if (InputManager::GetInstance()->GetState(static_cast<uint32>(Key::D)))
	{
		velocity += Vector3(defaultVelocity, 0, 0);
	}
	if (InputManager::GetInstance()->GetState(static_cast<uint32>(Key::W)))
	{
		velocity += Vector3(0, defaultVelocity, 0);
	}
	if (InputManager::GetInstance()->GetState(static_cast<uint32>(Key::S)))
	{
		velocity += Vector3(0, -defaultVelocity, 0);
	}

	(*pawn_).SetActorVelocity(velocity);
}

void PlayerController::handlePhysicsMoveFromUserInput()
{
	const float defaultForce = 0.1f;
	Vector3 force(0.0f, 0.0f, 0.0f);

	if (InputManager::GetInstance()->GetState(static_cast<uint32>(Key::A)))
	{
		force += Vector3(-defaultForce, 0, 0);
	}
	if (InputManager::GetInstance()->GetState(static_cast<uint32>(Key::D)))
	{
		force += Vector3(defaultForce, 0, 0);
	}
	if (InputManager::GetInstance()->GetState(static_cast<uint32>(Key::W)))
	{
		force += Vector3(0, defaultForce, 0);
	}
	if (InputManager::GetInstance()->GetState(static_cast<uint32>(Key::S)))
	{
		force += Vector3(0, -defaultForce, 0);
	}

	(*pawn_).AddForce(force);
}