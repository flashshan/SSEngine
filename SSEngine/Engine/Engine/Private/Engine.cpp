#include "Engine\Engine.h"

//#include "Core\Memory\HeapManager.h"
//#include "Core\String\StringPool.h"

#include "Controller\ControllerManager.h"
#include "Core\Memory\HeapManager.h"
#include "Manager\InputManager.h"
#include "Manager\WorldManager.h"
#include "SubSystem\Job\JobSystem.h"
#include "Manager\RealTimeManager.h"
#include "Manager\GameTimeManager.h"



Engine::Engine()
	: memoryBase_(nullptr), memorySize_(1024 * 1024 * 50), stringPoolSize_(1024 * 100)
{
}

Engine::~Engine()
{
}

void Engine::EngineStartup()
{
	engineMemoryInit();
	engineSubsystemInit();
	engineManagerInit();
}

void Engine::Run()
{
	WorldManager::GetInstance()->EarlyUpdate();

	ControllerManager::GetInstance()->UpdatePlayerController();
	ControllerManager::GetInstance()->UpdateMonsterController();

	PhysicsManager::GetInstance()->PhysicsUpdate();

	WorldManager::GetInstance()->Update();
	
	RenderManager::GetInstance()->RenderUpdate();

	WorldManager::GetInstance()->LateUpdate();
}

void Engine::EngineShutdown()
{
	engineManagerDestroy();
	engineSubsystemDestroy();
	engineMemoryFree();
}



void Engine::engineMemoryInit()
{
	memoryBase_ = _aligned_malloc(memorySize_, static_cast<size_t>(DEFAULT_ALIGNMENT));
	SLOW_ASSERT(memoryBase_, ErrorType::ENullPointer);

	HeapManager::CreateInstance(memoryBase_, memorySize_);

	void *stringPoolBase = HeapManager::GetInstance()->Alloc(stringPoolSize_);
	StringPool::CreateInstance(stringPoolBase, stringPoolSize_);
}

void Engine::engineSubsystemInit()
{
	JobSystem::CreateInstance();
	JobSystem::GetInstance()->CreateQueue("Default", 2);
	
	RenderManager::CreateInstance();
	PhysicsManager::CreateInstance();
}

void Engine::engineManagerInit()
{
	RealTimeManager::CreateInstance();
	GameTimeManager::CreateInstance();
	InputManager::CreateInstance();
	ControllerManager::CreateInstance();
	WorldManager::CreateInstance();
}

void Engine::engineManagerDestroy()
{
	WorldManager::DestroyInstance();
	ControllerManager::DestroyInstance();
	InputManager::DestroyInstance();
	GameTimeManager::DestroyInstance();
	RealTimeManager::DestroyInstance();
}

void Engine::engineSubsystemDestroy()
{
	PhysicsManager::DestroyInstance();
	RenderManager::DestroyInstance();

	JobSystem::GetInstance()->Shutdown();
	JobSystem::DestroyInstance();
}

void Engine::engineMemoryFree()
{
	StringPool::DestroyInstance();

	HeapManager::DestroyInstance();
	_aligned_free(memoryBase_);
}