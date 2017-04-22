#include "Jobs\JobLoadActor.h"

#include "Manager\WorldManager.h"

void JobLoadActor::Run()
{
	WorldManager::GetInstance()->SpawnActorFromLua<Actor>(luaFilePath_);
}