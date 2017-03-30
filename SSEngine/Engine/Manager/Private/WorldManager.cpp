#include "Manager\WorldManager.h"

#include "Core\Memory\New.h"

WorldManager *WorldManager::globalInstance_ = nullptr;

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
	if (type == typePlayer)
	{
		StrongPtr<Pawn> newPlayer = res;
		ControllerManager::GetInstance()->AddPlayerController(newPlayer);
		players_.push_back(newPlayer);
	}
	else if (type == typeRandomMoveMonster)
	{
		StrongPtr<Pawn> newRandomMoveMonster = res;
		StrongPtr<IController> newRandomMoveController = new RandomMoveController(newRandomMoveMonster);
		newRandomMoveMonster->SetController(newRandomMoveController);
		ControllerManager::GetInstance()->AddMonsterController(newRandomMoveController);
		pawns_.push_back(newRandomMoveMonster);
	}
	else if (type == typeFocusMoveMonster)
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


