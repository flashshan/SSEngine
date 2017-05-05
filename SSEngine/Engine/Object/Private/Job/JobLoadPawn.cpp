#include "Object\Job\JobLoadPawn.h"

#include "Manager\WorldManager.h"

void JobLoadPawn::Run()
{
	WorldManager::GetInstance()->SpawnPawnFromLua<Pawn>(luaFilePath_, spawnTransform_);
}
