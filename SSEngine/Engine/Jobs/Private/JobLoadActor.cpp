#include "Jobs\JobLoadActor.h"

#include "Manager\WorldManager.h"

void JobLoadActor::Run()
{
	WorldManager::GetInstance()->SpawnPawnFromLua<Pawn>(luaFilePath_);
}