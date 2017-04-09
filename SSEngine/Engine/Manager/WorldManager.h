#pragma once

#include <vector>

#include "Core\Template\Pointers.h"
#include "Core\Template\Array.h"
#include "Core\Template\Queue.h"
#include "Core\String\HashedString.h"

#include "Object\Pawn.h"

// singleton class
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

	template <typename T> WeakPtr<T> SpawnActor(const Transform &i_transform, const char *i_name, const char *i_type);
	template <typename T> WeakPtr<T> SpawnActorFromLua(const char *i_luaFileName);
	//typename std::enable_if<std::is_base_of<Pawn, T>::value>::type

	template <typename T> WeakPtr<T> SpawnPawn(const Transform &i_transform, const char *i_name, const char *i_type);
	template <typename T> WeakPtr<T> SpawnPawnFromLua(const char *i_luaFileName);

	void AddNewActorsIntoArray();
	void ActorsPreCalculation();
	void ActorsEarlyUpdate();
	void ActorsUpdate();
	void ActorsActualUpdate();
	void ActorsLateUpdate();
	
	FORCEINLINE size_t GetActorNumber() const { return actors_.Size(); }
	FORCEINLINE size_t GetPlayerNumber() const { return players_.Size(); }
	FORCEINLINE size_t GetPawnNumber() const { return pawns_.Size(); }

	FORCEINLINE WeakPtr<Actor> GetActor(const size_t i_index);
	FORCEINLINE WeakPtr<Pawn> GetPlayer(const size_t i_index);
	FORCEINLINE WeakPtr<Pawn> GetPawn(const size_t i_index);

private:
	FORCEINLINE WorldManager();
	FORCEINLINE WorldManager(const WorldManager &i_other) {}
	FORCEINLINE WorldManager& operator=(const WorldManager &i_other) {}

	static WorldManager *globalInstance_;

	template <typename T> WeakPtr<T> addActorToWorld(void *i_actor);
	template <typename T> WeakPtr<T> addPawnToWorld(void *i_pawn);
private:
	Array<StrongPtr<Actor>> actors_;
	Array<StrongPtr<Pawn>> players_;
	Array<StrongPtr<Pawn>> pawns_;

	Queue<StrongPtr<Actor>> newActors_;
	Queue<StrongPtr<Pawn>> newPlayers_;
	Queue<StrongPtr<Pawn>> newPawns_;

	CRITICAL_SECTION criticalSection_;
};





// implement forceinline

FORCEINLINE WorldManager *WorldManager::CreateInstance()
{
	ASSERT(WorldManager::globalInstance_ == nullptr);
	WorldManager::globalInstance_ = new TRACK_NEW WorldManager();
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
	WorldManager::globalInstance_ = nullptr;
}


FORCEINLINE WorldManager::WorldManager()
{
	InitializeCriticalSection(&criticalSection_);
}



FORCEINLINE WeakPtr<Actor> WorldManager::GetActor(const size_t i_index)
{
	ASSERT(i_index < actors_.Size());
	return WeakPtr<Actor>(actors_[i_index]);
}

FORCEINLINE WeakPtr<Pawn> WorldManager::GetPlayer(const size_t i_index)
{
	ASSERT(i_index < players_.Size() || !newPlayers_.Empty());
	if (i_index < players_.Size())
	{
		return WeakPtr<Pawn>(players_[i_index]);
	}
	else
	{
		return newPlayers_.Front();
	}
}

FORCEINLINE WeakPtr<Pawn> WorldManager::GetPawn(const size_t i_index)
{
	ASSERT(i_index < pawns_.Size());
	return WeakPtr<Pawn>(pawns_[i_index]);
}