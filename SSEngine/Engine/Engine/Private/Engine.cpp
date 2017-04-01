#include "Engine\Engine.h"

//#include "Core\Memory\HeapManager.h"
//#include "Core\String\StringPool.h"

#include "Controller\ControllerManager.h"
#include "Core\Memory\HeapManager.h"
#include "Manager\InputManager.h"
#include "Manager\WorldManager.h"
#include "Manager\RealTimeManager.h"
#include "Manager\GameTimeManager.h"



Engine::Engine()
{
}

Engine::~Engine()
{
}

void Engine::EngineInit()
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

void Engine::EngineQuit()
{
	engineManagerDestroy();
	engineSubsystemDestroy();
	engineMemoryFree();
}



void Engine::engineMemoryInit()
{
	memorySize_ = 1024 * 1024 * 10;
	memoryBase_ = _aligned_malloc(memorySize_, static_cast<size_t>(DEFAULT_ALIGNMENT));
	SLOW_ASSERT(memoryBase_, ErrorType::ENullPointer);

	HeapManager::CreateInstance(memoryBase_, memorySize_);

	const size_t stringPoolSize = 1024 * 100;
	void *stringPoolBase = HeapManager::GetInstance()->Alloc(stringPoolSize);
	StringPool::CreateInstance(stringPoolBase, stringPoolSize);
}

void Engine::engineSubsystemInit()
{
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
}

void Engine::engineMemoryFree()
{
	StringPool::DestroyInstance();

	HeapManager::DestroyInstance();
	_aligned_free(memoryBase_);
}