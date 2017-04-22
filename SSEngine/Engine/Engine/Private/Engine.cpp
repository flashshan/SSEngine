#include "Engine\Engine.h"

#include "Controller\ControllerManager.h"
#include "Core\Profile\ScopedTimer.h"
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

	// Add new actors created in last frame to array
	WorldManager::GetInstance()->AddNewActorsIntoArray();

	// Precalculate all actors
	PROFILE_GROUP_SCOPE_BEGIN("PreCalculation")
	WorldManager::GetInstance()->ActorsPreCalculation();
	PROFILE_GROUP_SCOPE_END()

	// Early update, user can override this function
	PROFILE_GROUP_SCOPE_BEGIN("EarlyUpdate")
	WorldManager::GetInstance()->ActorsEarlyUpdate();
	PROFILE_GROUP_SCOPE_END()

	// update players' controller and other actors/pawns controller
	PROFILE_GROUP_SCOPE_BEGIN("Controller")
	ControllerManager::GetInstance()->UpdatePlayerController();
	ControllerManager::GetInstance()->UpdateMonsterController();
	PROFILE_GROUP_SCOPE_END()

	// Physics subsystem works
	PROFILE_GROUP_SCOPE_BEGIN("Physics")
	PhysicsManager::GetInstance()->PhysicsUpdate();
	PROFILE_GROUP_SCOPE_END()

	// Update function, user can override this function
	PROFILE_GROUP_SCOPE_BEGIN("Update")
	WorldManager::GetInstance()->ActorsUpdate();
	PROFILE_GROUP_SCOPE_END()

	// Collision subsystem works
	PROFILE_GROUP_SCOPE_BEGIN("Collision")
	CollisionManager::GetInstance()->CollisionUpdate();
	PROFILE_GROUP_SCOPE_END()

	// actually update all actors gameeObject to change the property
	PROFILE_GROUP_SCOPE_BEGIN("ActualUpdate")
	WorldManager::GetInstance()->ActorsActualUpdate();
	PROFILE_GROUP_SCOPE_END()

	// Render subsystem works
	PROFILE_GROUP_SCOPE_BEGIN("Render")
	RenderManager::GetInstance()->RenderUpdate();
	PROFILE_GROUP_SCOPE_END()

	// Late update, user can override this function
	PROFILE_GROUP_SCOPE_BEGIN("LateUpdate")
	WorldManager::GetInstance()->ActorsLateUpdate();
	PROFILE_GROUP_SCOPE_END()

	// calculate frame time of this frame
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
	// Create memory heap and fixed size allocators
	memoryBase_ = _aligned_malloc(memorySize_, static_cast<size_t>(DEFAULT_ALIGNMENT));
	SLOW_ASSERT(memoryBase_, ErrorType::ENullPointer);
	HeapManager::CreateInstance(memoryBase_, memorySize_);

	// create string pool
	void *stringPoolBase = HeapManager::GetInstance()->Alloc(stringPoolSize_);
	StringPool::CreateInstance(stringPoolBase, stringPoolSize_);
}

void Engine::engineSubsystemInit()
{
	RealTimeManager::CreateInstance();
	GameTimeManager::CreateInstance();
	InputManager::CreateInstance();

	ProfilerManager::CreateInstance();

	JobSystem::CreateInstance();
	JobSystem::GetInstance()->CreateQueue("Default", 2);
	
	ResourceManager::CreateInstance();
	RenderManager::CreateInstance();
	PhysicsManager::CreateInstance();
	CollisionManager::CreateInstance();
}

void Engine::engineManagerInit()
{
	ControllerManager::CreateInstance();
	WorldManager::CreateInstance();
}

void Engine::engineManagerDestroy()
{
	WorldManager::DestroyInstance();
	ControllerManager::DestroyInstance();
}

void Engine::engineSubsystemDestroy()
{
	CollisionManager::DestroyInstance();
	PhysicsManager::DestroyInstance();
	RenderManager::DestroyInstance();
	ResourceManager::DestroyInstance();

	JobSystem::GetInstance()->Shutdown();
	JobSystem::DestroyInstance();

	ProfilerManager::DestroyInstance();

	InputManager::DestroyInstance();
	GameTimeManager::DestroyInstance();
	RealTimeManager::DestroyInstance();
}

void Engine::engineMemoryFree()
{
	StringPool::DestroyInstance();

	HeapManager::DestroyInstance();
	_aligned_free(memoryBase_);
}