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
	RealTimeManager::GetInstance()->SetLastTimeMark();

	WorldManager::GetInstance()->AddNewActorsIntoArray();

	WorldManager::GetInstance()->ActorsPreCalculation();
	WorldManager::GetInstance()->ActorsEarlyUpdate();

	ControllerManager::GetInstance()->UpdatePlayerController();
	ControllerManager::GetInstance()->UpdateMonsterController();

	PhysicsManager::GetInstance()->PhysicsUpdate();

	WorldManager::GetInstance()->ActorsUpdate();
	
	WorldManager::GetInstance()->ActorsActualUpdate();

	RenderManager::GetInstance()->RenderUpdate();

	WorldManager::GetInstance()->ActorsLateUpdate();

	// calculate frame time
	RealTimeManager::GetInstance()->CalcLastFrameTime_ms();

	float waitTime = DESIRED_FRAMETIME_MS - RealTimeManager::GetInstance()->GetLastMarkTimeMS();
	//DEBUG_PRINT("Frame time: %f", waitTime);

	if (waitTime > 0.0f)
	{
		DWORD time = static_cast<DWORD>(waitTime);
		//DEBUG_PRINT("Wait time: %ld", time);
		Sleep(time);
	}

	
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