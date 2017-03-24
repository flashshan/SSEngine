#pragma once

#include "Engine\Controller\IGameObjectController.h"
#include "Engine\Object\GameObject.h"
#include "Engine\Manager\RealTimeManager.h"


//#include <utility>
class FocusMoveMonsterController : public IGameObjectController
{
public:

	FORCEINLINE FocusMoveMonsterController();
	FORCEINLINE FocusMoveMonsterController(Vector3 i_Position);
	FORCEINLINE FocusMoveMonsterController(const FocusMoveMonsterController & i_OtherController);
	FORCEINLINE FocusMoveMonsterController(FocusMoveMonsterController && i_OtherController);
	FORCEINLINE FocusMoveMonsterController & operator= (const FocusMoveMonsterController & i_OtherController);
	FORCEINLINE FocusMoveMonsterController & operator= (FocusMoveMonsterController && i_OtherController);

	FORCEINLINE ~FocusMoveMonsterController();

	virtual FORCEINLINE void SetGameObject(StrongPtr<GameObject> i_GameObject) override { gameObject_ = i_GameObject; }
	virtual FORCEINLINE StrongPtr<GameObject> GetGameObject() const override { return gameObject_; }
	
	FORCEINLINE void setFocusObject(StrongPtr<GameObject> i_GameObject) { focusObject_ = i_GameObject; }
	
	virtual FORCEINLINE Vector3 GetPosition() const override { return gameObject_->GetPosition(); };

	FORCEINLINE Vector3 GetMovementDirectionFromFocus() const;
	virtual FORCEINLINE void UpdateGameObject() override;
	
private:
	StrongPtr<GameObject> gameObject_;
	StrongPtr<GameObject> focusObject_;
};









// implement forceinline

FORCEINLINE FocusMoveMonsterController::FocusMoveMonsterController() 
	: gameObject_(new GameObject((Vector3::RandomNormal() * (float)(rand() % 800)))) 
{
}

FORCEINLINE FocusMoveMonsterController::FocusMoveMonsterController(Vector3 i_Position) 
	: gameObject_(new GameObject(i_Position)) 
{
}

FORCEINLINE FocusMoveMonsterController::FocusMoveMonsterController(const FocusMoveMonsterController & i_OtherController) 
	: gameObject_(new GameObject(i_OtherController.GetPosition())), focusObject_(i_OtherController.focusObject_) 
{
}

FORCEINLINE FocusMoveMonsterController::FocusMoveMonsterController(FocusMoveMonsterController && i_OtherController)
	: gameObject_(i_OtherController.gameObject_), focusObject_(i_OtherController.focusObject_)
{
}

FORCEINLINE FocusMoveMonsterController & FocusMoveMonsterController::operator= (const FocusMoveMonsterController & i_OtherController)
{
	gameObject_ = new GameObject(i_OtherController.GetPosition());
	focusObject_ = i_OtherController.focusObject_;
}
FORCEINLINE FocusMoveMonsterController & FocusMoveMonsterController::operator= (FocusMoveMonsterController && i_OtherController)
{
	std::swap(gameObject_, i_OtherController.gameObject_);
	std::swap(focusObject_, i_OtherController.focusObject_);
}

FORCEINLINE FocusMoveMonsterController::~FocusMoveMonsterController()
{
}

FORCEINLINE void FocusMoveMonsterController::UpdateGameObject()
{
	if (gameObject_)
	{
		Vector3 Direction;
		if (focusObject_)
			Direction = GetMovementDirectionFromFocus();

		gameObject_->SetPosition(gameObject_->GetPosition() + Direction * RealTimeManager::GetInstance()->GetLastFrameTimeS());
	}
}

FORCEINLINE Vector3 FocusMoveMonsterController::GetMovementDirectionFromFocus() const
{
	if (!focusObject_ || !gameObject_) return Vector3(0, 0, 0);

	Vector3 res = focusObject_->GetPosition() - gameObject_->GetPosition();
	res.Normalize();
	return res;
}