#include "Jobs\JobLoadPawn.h"

#include "Manager\WorldManager.h"

void JobLoadPawn::Run()
{
	WorldManager::GetInstance()->SpawnPawnFromLua<Pawn>(luaFilePath_);
}
