#pragma once

#include <vector>

#include "Controller\FocusMoveController.h"
#include "Controller\RandomMoveController.h"
#include "Controller\ControllerManager.h"
#include "Object\Pawn.h"

class WorldManager
{
public:
	const HashedString typePlayer = HashedString("Player");
	const HashedString typeRandomMoveMonster = HashedString("RandomMoveMonster");
	const HashedString typeFocusMoveMonster = HashedString("FocusMoveMonster");

public:
	static FORCEINLINE WorldManager *CreateInstance();
	static FORCEINLINE WorldManager *GetInstance();
	static FORCEINLINE void DestroyInstance();

	~WorldManager();

	template <typename T> T* SpawnActor(const Transform &i_transform, const char *i_name, const char *i_type);
	//typename std::enable_if<std::is_base_of<Pawn, T>::value>::type

	template <typename T> T* SpawnPawn(const Transform &i_transform, const char *i_name, const char *i_type);
	
	void EarlyUpdate();
	void Update();
	void LateUpdate();

	FORCEINLINE StrongPtr<Pawn> GetPlayer(const uint32 i_index);

private:
	FORCEINLINE WorldManager();
	FORCEINLINE WorldManager(const WorldManager &i_other) {}

	static WorldManager *globalInstance_;

private:
	std::vector<StrongPtr<Pawn>> players_;
	std::vector<StrongPtr<Actor>> actors_;
	std::vector<StrongPtr<Pawn>> pawns_;
};





// implement forceinline

FORCEINLINE WorldManager *WorldManager::CreateInstance()
{
	ASSERT(WorldManager::globalInstance_ == nullptr);
	WorldManager::globalInstance_ = new WorldManager();
	return WorldManager::globalInstance_;
}

FORCEINLINE WorldManager *WorldManager::GetInstance()
{
	ASSERT(WorldManager::globalInstance_ != nullptr);
	return WorldManager::globalInstance_;
}

FORCEINLINE void WorldManager::DestroyInstance()
{
	ASSERT(WorldManager::globalInstance_ != nullptr);
	delete WorldManager::globalInstance_;
}


FORCEINLINE WorldManager::WorldManager()
{
}

FORCEINLINE StrongPtr<Pawn> WorldManager::GetPlayer(const uint32 i_index)
{
	ASSERT(i_index < players_.size());
	return players_[i_index];
}


