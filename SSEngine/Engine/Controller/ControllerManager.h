#pragma once

#include <vector>

#include "Core\Template\Pointers.h"
#include "Core\Template\Array.h"
#include "Core\Memory\New.h"
#include "PlayerController.h"
#include "Object\Pawn.h"

// singleton class
class ControllerManager
{
public:
	static FORCEINLINE ControllerManager *CreateInstance();
	static FORCEINLINE ControllerManager *GetInstance();
	static FORCEINLINE void DestroyInstance();

	inline ~ControllerManager();

	FORCEINLINE void AddPlayerController(const WeakPtr<Pawn> i_player);
	FORCEINLINE void AddMonsterController(const StrongPtr<IController> i_controller);
	
	void RemoveMonsterController(WeakPtr<IController> i_monsterController);
	FORCEINLINE void RemoveMonsterControllerByIndex(const int32 i_index);

	FORCEINLINE StrongPtr<PlayerController> GetPlayerController(const uint32 i_index);

	void UpdatePlayerController();
	void UpdateMonsterController();

private:
	FORCEINLINE ControllerManager();
	FORCEINLINE ControllerManager(const ControllerManager &i_other) {}
	FORCEINLINE ControllerManager& operator=(const ControllerManager &i_other) {}

	static ControllerManager *globalInstance_;

private:
	Array<StrongPtr<PlayerController>> playerControllers_;
	Array<StrongPtr<IController>> monsterControllers_;

	CRITICAL_SECTION criticalSection_;
};





// implement

FORCEINLINE ControllerManager::ControllerManager()
{
	InitializeCriticalSection(&criticalSection_);
}

FORCEINLINE ControllerManager *ControllerManager::CreateInstance()
{
	ASSERT(ControllerManager::globalInstance_ == nullptr);
	ControllerManager::globalInstance_ = new TRACK_NEW ControllerManager();
	return ControllerManager::globalInstance_;
}

FORCEINLINE ControllerManager *ControllerManager::GetInstance()
{
	ASSERT(ControllerManager::globalInstance_ != nullptr);
	return ControllerManager::globalInstance_;
}

FORCEINLINE void ControllerManager::DestroyInstance()
{
	ASSERT(ControllerManager::globalInstance_ != nullptr);
	delete ControllerManager::globalInstance_;
	ControllerManager::globalInstance_ = nullptr;
}

inline ControllerManager::~ControllerManager()
{
	playerControllers_.Clear();
	monsterControllers_.Clear();
}

FORCEINLINE void ControllerManager::AddPlayerController(const WeakPtr<Pawn> i_player)
{
	StrongPtr<PlayerController> newPlayerController = new TRACK_NEW PlayerController(i_player);

	(*i_player).SetController(CastStrongPtr<PlayerController, IController>(newPlayerController));
	EnterCriticalSection(&criticalSection_);
	playerControllers_.Add(newPlayerController);
	LeaveCriticalSection(&criticalSection_);
}

FORCEINLINE void ControllerManager::AddMonsterController(const StrongPtr<IController> i_monsterController)
{
	EnterCriticalSection(&criticalSection_);
	monsterControllers_.Add(i_monsterController);
	LeaveCriticalSection(&criticalSection_);
}

FORCEINLINE void ControllerManager::RemoveMonsterControllerByIndex(const int32 i_index)
{
	EnterCriticalSection(&criticalSection_);
	monsterControllers_.NoOrderRemove(i_index);
	LeaveCriticalSection(&criticalSection_);
}

FORCEINLINE StrongPtr<PlayerController> ControllerManager::GetPlayerController(const uint32 i_index)
{
	ASSERT(i_index < playerControllers_.Size());
	return playerControllers_[i_index];
}

