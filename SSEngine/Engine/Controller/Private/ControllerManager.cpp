#include "Controller\ControllerManager.h"


ControllerManager *ControllerManager::globalInstance_ = nullptr;

void ControllerManager::RemoveMonsterController(WeakPtr<IController> i_monsterController)
{
	EnterCriticalSection(&criticalSection_);
	uint32 size = static_cast<uint32>(monsterControllers_.Size());
	
	for (uint32 i = 0; i < size; ++i)
	{
		if (monsterControllers_[i])
		{
			if (&(*monsterControllers_[i]) == &(*i_monsterController))
			{
				monsterControllers_.NoOrderRemove(i);
				LeaveCriticalSection(&criticalSection_);
				return;
			}
		}
	}
	LeaveCriticalSection(&criticalSection_);
}

void ControllerManager::UpdatePlayerController()
{
	EnterCriticalSection(&criticalSection_);
	uint32 size = static_cast<uint32>(playerControllers_.Size());
	for (uint32 i = 0; i < size; ++i)
	{
		if (playerControllers_[i]->GetPawn()->GetActive())
		{
			playerControllers_[i]->UpdateController();
		}
	}
	LeaveCriticalSection(&criticalSection_);
}

void ControllerManager::UpdateMonsterController()
{
	EnterCriticalSection(&criticalSection_);
	int32 size = static_cast<uint32>(monsterControllers_.Size());
	for (int32 i = 0;i < size;++i)
	{
		if (monsterControllers_[i]->IsValid())
		{
			if (monsterControllers_[i]->GetPawn()->GetActive())
			{
				monsterControllers_[i]->UpdateController();
			}
		}
		else
		{
			RemoveMonsterControllerByIndex(i);
			i--;
		}
	}
	LeaveCriticalSection(&criticalSection_);
}
