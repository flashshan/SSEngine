#include "Controller\ControllerManager.h"


ControllerManager *ControllerManager::globalInstance_ = nullptr;

void ControllerManager::RemoveMonsterController(IController &i_monsterController)
{
	uint32 size = static_cast<uint32>(monsterControllers_.size());
	for (uint32 i = 0; i < size; ++i)
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

void ControllerManager::UpdatePlayerController()
{
	uint32 size = static_cast<uint32>(playerControllers_.size());
	for (uint32 i = 0;i < size;++i)
	{
		playerControllers_[i]->UpdateController();
	}
}

void ControllerManager::UpdateMonsterController()
{
	int32 size = static_cast<uint32>(monsterControllers_.size());
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
