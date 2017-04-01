#pragma once

#include <vector>

#include "Core\Template\Pointers.h"
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

	FORCEINLINE void AddPlayerController(const StrongPtr<Pawn> &i_player);
	FORCEINLINE void AddMonsterController(const StrongPtr<IController> &i_controller);
	
	void RemoveMonsterController(IController &i_monsterController);
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
	std::vector<StrongPtr<PlayerController>> playerControllers_;
	std::vector<StrongPtr<IController>> monsterControllers_;
};





// implement

FORCEINLINE ControllerManager::ControllerManager()
{
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
	playerControllers_.clear();
	monsterControllers_.clear();
}

FORCEINLINE void ControllerManager::AddPlayerController(const StrongPtr<Pawn> &i_player)
{
	StrongPtr<PlayerController> newPlayerController = new TRACK_NEW PlayerController(i_player);
	(*i_player).SetController(CastStrongPtr<PlayerController, IController>(newPlayerController));
	playerControllers_.push_back(newPlayerController);
}

FORCEINLINE void ControllerManager::AddMonsterController(const StrongPtr<IController> &i_monsterController)
{
	monsterControllers_.push_back(i_monsterController);
}

FORCEINLINE void ControllerManager::RemoveMonsterControllerByIndex(const int32 i_index)
{
	Basic::Swap(monsterControllers_[i_index], monsterControllers_[monsterControllers_.size() - 1]);
	monsterControllers_.pop_back();
}

FORCEINLINE StrongPtr<PlayerController> ControllerManager::GetPlayerController(const uint32 i_index)
{
	ASSERT(i_index < playerControllers_.size());
	return playerControllers_[i_index];
}

