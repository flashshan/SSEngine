#pragma once

#include <vector>

#include "Core\Template\Pointers.h"
#include "PlayerController.h"
#include "Core\String\HashedString.h"
#include "Object\Pawn.h"

class ControllerManager
{
public:
	static FORCEINLINE ControllerManager *GetInstance();
	inline ~ControllerManager();

	FORCEINLINE void AddPlayerController(const StrongPtr<Pawn> &i_player);
	FORCEINLINE void AddMonsterController(const StrongPtr<IController> &i_controller);
	void RemoveMonsterController(IController &i_monsterController);
	
	FORCEINLINE void RemoveMonsterControllerByIndex(const int32 i_index);

	FORCEINLINE StrongPtr<IController> GetPlayerController(const uint32 i_index);

	void UpdatePlayerController();
	void UpdateMonsterController();

private:
	FORCEINLINE ControllerManager();

private:
	std::vector<StrongPtr<IController>> playerControllers_;
	std::vector<StrongPtr<IController>> monsterControllers_;
};




FORCEINLINE ControllerManager::ControllerManager()
{

}

// implement
FORCEINLINE ControllerManager *ControllerManager::GetInstance()
{
	static ControllerManager *globalInstance;
	if (globalInstance == nullptr)
	{
		globalInstance = new ControllerManager();
	}
	return globalInstance;
}


inline ControllerManager::~ControllerManager()
{
	playerControllers_.clear();
	monsterControllers_.clear();
}

FORCEINLINE void ControllerManager::AddPlayerController(const StrongPtr<Pawn> &i_player)
{
	StrongPtr<IController> newPlayerController = new PlayerController(i_player);
	(*i_player).SetController(newPlayerController);
	playerControllers_.push_back(newPlayerController);
}

FORCEINLINE void ControllerManager::AddMonsterController(const StrongPtr<IController> &i_monsterController)
{
	monsterControllers_.push_back(i_monsterController);
}

void ControllerManager::RemoveMonsterController(IController &i_monsterController)
{
	int32 size = static_cast<int32>(monsterControllers_.size());
	for (int32 i = 0; i < size; ++i)
	{
		if (monsterControllers_[i])
		{
			if (&(*monsterControllers_[i]) == &i_monsterController)
			{
				Basic::Swap(monsterControllers_[i], monsterControllers_[size - 1]);
				monsterControllers_.pop_back();
				break;
			}
		}
	}
}

FORCEINLINE void ControllerManager::RemoveMonsterControllerByIndex(const int32 i_index)
{
	Basic::Swap(monsterControllers_[i_index], monsterControllers_[monsterControllers_.size() - 1]);
	monsterControllers_.pop_back();
}

FORCEINLINE StrongPtr<IController> ControllerManager::GetPlayerController(const uint32 i_index)
{
	ASSERT(i_index < playerControllers_.size());
	return playerControllers_[i_index];
}

void ControllerManager::UpdatePlayerController()
{
	int32 size = static_cast<int32>(playerControllers_.size());
	for (int32 i = 0;i < size;++i)
	{
		playerControllers_[i]->UpdateController();
	}
}

void ControllerManager::UpdateMonsterController()
{
	int32 size = static_cast<int32>(monsterControllers_.size());
	for (int32 i = 0;i < size;++i)
	{
		if ((*monsterControllers_[i]).IsValid())
		{
			monsterControllers_[i]->UpdateController();
		}
		else
		{
			RemoveMonsterControllerByIndex(i);
			i--;
		}
	}
}
