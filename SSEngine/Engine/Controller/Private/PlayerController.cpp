#include "Controller\PlayerController.h"

#include "Manager\InputManager.h"
#include "Object\Entity\Pawn.h"
#include "Manager\WorldManager.h"

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
		createBulletFromInput();
	}
}

void PlayerController::handleBasicMoveFromUserInput() const
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

	pawn_->SetActorVelocity(velocity);
}

void PlayerController::handlePhysicsMoveFromUserInput() const
{
	Vector3 force(0.0f, 0.0f, 0.0f);

	if (InputManager::GetInstance()->GetState(static_cast<uint32>(Key::A)))
	{
		force += Vector3(-1, 0, 0);
	}
	if (InputManager::GetInstance()->GetState(static_cast<uint32>(Key::D)))
	{
		force += Vector3(1, 0, 0);
	}
	if (InputManager::GetInstance()->GetState(static_cast<uint32>(Key::W)))
	{
		force += Vector3(0, 1, 0);
	}
	if (InputManager::GetInstance()->GetState(static_cast<uint32>(Key::S)))
	{
		force += Vector3(0, -1, 0);
	}
	const float defaultForce = 100.0f;
	force *= defaultForce;

	pawn_->AddForce(force);
}

// hardcode stuff
void PlayerController::createBulletFromInput() const
{
	static bool state = false;

	if (!state && InputManager::GetInstance()->GetState(static_cast<uint32>(Key::J)))
	{
		// press event

		Vector3 spawnVector(0.f, 0.f, 0.f);
		bool operateMark = false;
		if (InputManager::GetInstance()->GetState(static_cast<uint32>(Key::A)))
		{
			spawnVector += Vector3(-1, 0, 0);
			operateMark = true;
		}
		if (InputManager::GetInstance()->GetState(static_cast<uint32>(Key::D)))
		{
			spawnVector += Vector3(1, 0, 0);
			operateMark = true;
		}
		if (InputManager::GetInstance()->GetState(static_cast<uint32>(Key::W)))
		{
			spawnVector += Vector3(0, 1, 0);
			operateMark = true;
		}
		if (InputManager::GetInstance()->GetState(static_cast<uint32>(Key::S)))
		{
			spawnVector += Vector3(0, -1, 0);
			operateMark = true;
		}
		const float defaultDistance = 20.0f;
		const float defaultVelocity = 200.f;

		if (operateMark)
		{
			WeakPtr<Actor> bullet = WorldManager::GetInstance()->SpawnActorFromLua<Actor>("Assets\\Data\\Bullet.lua", Transform(pawn_->GetActorLocation() + spawnVector * defaultDistance));
			bullet->SetActorVelocity(spawnVector * defaultVelocity);
		}

		state = true;
	}
	else if (state && !InputManager::GetInstance()->GetState(static_cast<uint32>(Key::J)))
	{
		// release event
		state = false;
	}
}