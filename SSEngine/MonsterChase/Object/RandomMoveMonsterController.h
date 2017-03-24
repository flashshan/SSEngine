#pragma once

#include "Engine/Controller/IGameObjectController.h"
#include "Engine\Object\GameObject.h"
#include "Engine\Manager\RealTimeManager.h"

#include <utility>


class RandomMoveMonsterController : public IGameObjectController
{
public:
	const int32 DefaultScreenSize = 800;

	FORCEINLINE RandomMoveMonsterController();
	explicit FORCEINLINE RandomMoveMonsterController(Vector3 i_Position);

	FORCEINLINE RandomMoveMonsterController(const RandomMoveMonsterController & i_OtherController);
	FORCEINLINE RandomMoveMonsterController(RandomMoveMonsterController && i_OtherController);
	FORCEINLINE RandomMoveMonsterController & operator= (const RandomMoveMonsterController & i_OtherController);
	FORCEINLINE RandomMoveMonsterController & operator= (RandomMoveMonsterController && i_OtherController);

	FORCEINLINE RandomMoveMonsterController::~RandomMoveMonsterController();

	virtual FORCEINLINE void SetGameObject(StrongPtr<GameObject> i_GameObject) override { gameObject_ = i_GameObject; }
	virtual FORCEINLINE StrongPtr<GameObject> GetGameObject() const override { return gameObject_; }
	virtual FORCEINLINE Vector3 GetPosition() const override { return gameObject_->GetPosition(); };

	FORCEINLINE Vector3 GetMovementDirectionFromRandom() const;
	virtual FORCEINLINE void UpdateGameObject() override;
	
private:
	StrongPtr<GameObject> gameObject_;
};









// implement forceinline

FORCEINLINE RandomMoveMonsterController::RandomMoveMonsterController() 
	: gameObject_(new GameObject(Vector3::RandomNormal() * (float)(rand() % DefaultScreenSize))) 
{
}

FORCEINLINE RandomMoveMonsterController::RandomMoveMonsterController(Vector3 i_Position) 
	: gameObject_(new GameObject(i_Position)) 
{
}

FORCEINLINE RandomMoveMonsterController::RandomMoveMonsterController(const RandomMoveMonsterController & i_OtherController) 
	: gameObject_(new GameObject(i_OtherController.GetPosition())) 
{
}

FORCEINLINE RandomMoveMonsterController::RandomMoveMonsterController(RandomMoveMonsterController && i_OtherController) 
	: gameObject_(i_OtherController.gameObject_) 
{
}

FORCEINLINE RandomMoveMonsterController & RandomMoveMonsterController::operator= (const RandomMoveMonsterController & i_OtherController)
{
	gameObject_ = new GameObject(i_OtherController.GetPosition());
}

FORCEINLINE RandomMoveMonsterController & RandomMoveMonsterController::operator= (RandomMoveMonsterController && i_OtherController)
{
	std::swap(gameObject_, i_OtherController.gameObject_);
}

FORCEINLINE RandomMoveMonsterController::~RandomMoveMonsterController()
{
}

FORCEINLINE Vector3 RandomMoveMonsterController::GetMovementDirectionFromRandom() const
{
	return Vector3::RandomNormal();
}

FORCEINLINE void RandomMoveMonsterController::UpdateGameObject()
{
	if (gameObject_)
	{
		Vector3 Direction = GetMovementDirectionFromRandom();
		gameObject_->SetPosition(gameObject_->GetPosition() + Direction * RealTimeManager::GetInstance()->GetLastFrameTimeS());
	}
}
