#include "Object\Job\JobLoadActor.h"

#include "Manager\WorldManager.h"

void JobLoadActor::Run()
{
	WorldManager::GetInstance()->SpawnActorFromLua<Actor>(luaFilePath_, spawnTransform_);
}