#include "Controller\ControllerManager.h"


ControllerManager *ControllerManager::globalInstance_ = nullptr;

void ControllerManager::RemoveMonsterController(const WeakPtr<IController> &i_monsterController)
{
	EnterCriticalSection(&criticalSection_);
	
	size_t count = monsterControllers_.Size();
	for (size_t i = 0; i < count; ++i)
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
	for (size_t i = 0; i < playerControllers_.Size(); ++i)
	{
		if (playerControllers_[i]->GetControlledPawn()->GetActive())
		{
			playerControllers_[i]->UpdateController();
		}
	}
	LeaveCriticalSection(&criticalSection_);
}

void ControllerManager::UpdateMonsterController()
{
	EnterCriticalSection(&criticalSection_);
	for (size_t i = 0;i < monsterControllers_.Size();++i)
	{
		if (monsterControllers_[i]->IsValid())
		{
			if (monsterControllers_[i]->GetControlledPawn()->GetActive())
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
