#include "Controller\PlayerController.h"

#include "SubSystem\InputManager.h"
#include "Manager\RealTimeManager.h"

PlayerController::~PlayerController()
{
}

void PlayerController::UpdateGameObject()
{
	const float weight = 50.0f;
	Vector3 accerlerate = forceDirection_ / weight;
	const float lastFrameSecond = RealTimeManager::GetInstance()->GetLastFrameTimeS();
	if (gameObject_)
	{
		gameObject_->SetPosition(gameObject_->GetPosition() + velocity_ * lastFrameSecond
		+ accerlerate * lastFrameSecond * lastFrameSecond * 0.5);

		velocity_ += accerlerate * lastFrameSecond;
	}
}

void PlayerController::GetMovementDirectionFromUserInput()
{
	forceDirection_ = Vector3(0, 0, 0);
	
	if (InputManager::GetInstance()->GetState(Key::A))
	{
		forceDirection_ += Vector3(-1, 0, 0);
	}
	if (InputManager::GetInstance()->GetState(Key::D))
	{
		forceDirection_ += Vector3(1, 0, 0);
	}
	if (InputManager::GetInstance()->GetState(Key::W))
	{
		forceDirection_ += Vector3(0, 1, 0);
	}
	if (InputManager::GetInstance()->GetState(Key::S))
	{
		forceDirection_ += Vector3(0, -1, 0);
	}
}