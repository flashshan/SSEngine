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
	static FORCEINLINE WorldManager *GetInstance();
	~WorldManager();

	template <typename T> T* SpawnActor(const Transform &i_transform, const char *i_name, const char *i_type);
	//typename std::enable_if<std::is_base_of<Pawn, T>::value>::type

	template <typename T> T* SpawnPawn(const Transform &i_transform, const char *i_name, const char *i_type);
	
	void EarlyUpdate();
	void Update();
	void LateUpdate();

	StrongPtr<Pawn> GetPlayer(const uint32 i_index);

private:
	FORCEINLINE WorldManager();

	// world can not be copyed
	WorldManager(const WorldManager &i_other) {}

private:
	std::vector<StrongPtr<Pawn>> players_;
	std::vector<StrongPtr<Actor>> actors_;
	std::vector<StrongPtr<Pawn>> pawns_;
};





// implement forceinline

FORCEINLINE WorldManager* WorldManager::GetInstance()
{
	static WorldManager *globalInstance;
	if (globalInstance == nullptr)
	{
		globalInstance = new WorldManager();
	}
	return globalInstance;
}

WorldManager::~WorldManager()
{
	pawns_.clear();
	actors_.clear();
	players_.clear();
}

template<> Actor* WorldManager::SpawnActor<Actor>(const Transform &i_transform, const char *i_name, const char *i_type)
{
	Actor *res = new Actor(i_transform, i_name, i_type);
	StrongPtr<Actor> newActor = res;
	actors_.push_back(newActor);
	return res;
}


// spawn pawn according to some build in types
template<> Pawn* WorldManager::SpawnPawn<Pawn>(const Transform &i_transform, const char *i_name, const char *i_type)
{
	Pawn *res = new Pawn(i_transform, i_name, i_type);
	HashedString type(i_type);
	if(type == typePlayer)
	{
		StrongPtr<Pawn> newPlayer = res;
		ControllerManager::GetInstance()->AddPlayerController(newPlayer);
		players_.push_back(newPlayer);
	}
	else if(type == typeRandomMoveMonster)
	{
		StrongPtr<Pawn> newRandomMoveMonster = res;
		StrongPtr<IController> newRandomMoveController = new RandomMoveController(newRandomMoveMonster);
		newRandomMoveMonster->SetController(newRandomMoveController);
		ControllerManager::GetInstance()->AddMonsterController(newRandomMoveController);
		pawns_.push_back(newRandomMoveMonster);
	}
	else if(type == typeFocusMoveMonster)
	{
		StrongPtr<Pawn> newFocusMoveMonster = res;
		StrongPtr<IController> newFocusMoveController = new FocusMoveController(newFocusMoveMonster, GetPlayer(0));
		newFocusMoveMonster->SetController(newFocusMoveController);
		ControllerManager::GetInstance()->AddMonsterController(newFocusMoveController);
		pawns_.push_back(newFocusMoveMonster);
	}
	else
	{
		StrongPtr<Pawn> newMonster = res;
		pawns_.push_back(newMonster);
	}
	return res;
}


FORCEINLINE WorldManager::WorldManager()
{
}

StrongPtr<Pawn> WorldManager::GetPlayer(const uint32 i_index)
{
	ASSERT(i_index < players_.size());
	return players_[i_index];
}

void WorldManager::EarlyUpdate()
{
	for (auto && x : actors_)
	{
		x->EarlyUpdate();
	}
	for (auto && x : players_)
	{
		x->EarlyUpdate();
	}
	for (auto && x : pawns_)
	{
		x->EarlyUpdate();
	}
}

void WorldManager::Update()
{
	for (auto && x : actors_)
	{
		x->Update();
	}
	for (auto && x : players_)
	{
		x->Update();
	}
	for (auto && x : pawns_)
	{
		x->Update();
	}
}

void WorldManager::LateUpdate()
{
	for (auto && x : actors_)
	{
		x->LateUpdate();
	}
	for (auto && x : players_)
	{
		x->LateUpdate();
	}
	for (auto && x : pawns_)
	{
		x->LateUpdate();
	}
}




