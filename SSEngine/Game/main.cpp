// main.cpp : Defines the entry point for the application.

#include <Windows.h>
#include <stdio.h>
#include <stdint.h>
#include <memory.h>

#include <conio.h>
#include <time.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>

#include "Engine\Manager\InputManager.h"
#include "Engine\Manager\RealTimeManager.h"
#include "Manager\WorldManager.h"
#include "Engine\Engine.h"

#include "SubSystem\Job\JobSystem.h"
#include "Object\Job\JobLoadActor.h"
#include "Object\Job\JobLoadPawn.h"

#include "Glib\GLib.h"



#include "GLib\GLib.h"
#include "Lua/lua.hpp"


//#define RANDOM_MOVE_MONSTER
#define FOCUS_MOVE_MONSTER

#define PlAYER_NUMBER 1

#define FOCUS_MONSTER_NUMBER 10
#define RANDOM_MONSTER_NUMBER 1


void TestKeyCallback(unsigned int i_VKeyID, bool bWentDown)
{
#ifdef _DEBUG
	/*const size_t	lenBuffer = MAX_NAME_LENGTH;
	char			Buffer[lenBuffer];*/

	//sprintf_s(Buffer, lenBuffer, "VKey 0x%04x went %s\n", i_VKeyID, bWentDown ? "down" : "up");
	//DEBUG_PRINT("%s", Buffer);
#endif // __DEBUG
	InputManager::GetInstance()->SetState(i_VKeyID, bWentDown);
}


int WINAPI wWinMain(HINSTANCE i_hInstance, HINSTANCE i_hPrevInstance, LPWSTR i_lpCmdLine, int i_nCmdShow)
{
	Engine engine;
	engine.EngineStartup();

	// first we need to initialize GLib
	bool bSuccess = GLib::Initialize(i_hInstance, i_nCmdShow, "Game", -1, 800, 600);

	const float playerInitDistance = 50;
	const float monsterInitDistance = 200;

	if (bSuccess)
	{
		// calculate last frame time
		RealTimeManager::GetInstance()->CalcLastFrameTime_ms();

		// Set a callback for notification of key presses
		GLib::SetKeyStateChangeCallback(TestKeyCallback);

		srand(static_cast<unsigned int>(time(0)));

		const int32 playerNumber = PlAYER_NUMBER;

		// assign player
		WorldManager::GetInstance()->SpawnPawnFromLua<Pawn>("Assets\\Data\\Player1.lua", Transform(Vector3(0.f, 0.f, 0.f)));
		/*for (int32 i = 0; i < playerNumber; ++i)
		{
			JobLoadPawn *jobLoadPlayer = new JobLoadPawn("Assets\\Data\\Player1.lua", Transform(Vector3(0.f, 0.f, 0.f)));
			ASSERT(jobLoadPlayer);
			JobSystem::GetInstance()->RunJob(*jobLoadPlayer, "Default");
		}*/

		// spawn walls

		JobLoadActor *jobLoadWall = new TRACK_NEW JobLoadActor("Assets\\Data\\LeftWall.lua", Transform(Vector3(-380.f, -300.f, 0.f)));
		ASSERT(jobLoadWall);
		JobSystem::GetInstance()->RunJob(*jobLoadWall, "Default");
		jobLoadWall = new TRACK_NEW JobLoadActor("Assets\\Data\\RightWall.lua", Transform(Vector3(380.f, -300.f, 0.f)));
		ASSERT(jobLoadWall);
		JobSystem::GetInstance()->RunJob(*jobLoadWall, "Default");
		jobLoadWall = new TRACK_NEW JobLoadActor("Assets\\Data\\TopWall.lua", Transform(Vector3(0.f, 263.f, 0.f)));
		ASSERT(jobLoadWall);
		JobSystem::GetInstance()->RunJob(*jobLoadWall, "Default");
		jobLoadWall = new TRACK_NEW JobLoadActor("Assets\\Data\\BottomWall.lua", Transform(Vector3(0.f, -293.f, 0.f)));
		ASSERT(jobLoadWall);
		JobSystem::GetInstance()->RunJob(*jobLoadWall, "Default");

		// assign monsters
		const int32 focusMonsterNumber = FOCUS_MONSTER_NUMBER;

		const int32 randomMonsterNumber = RANDOM_MONSTER_NUMBER;

		Vector3 focusMonsterLocation[focusMonsterNumber];
		const float XPos = -300, XInterval = 150, YPos1 = 200, YPos2 = -200;
		for (int32 i = 0;i < focusMonsterNumber / 2; ++i)
		{
			focusMonsterLocation[i] = Vector3(XPos + XInterval * i, YPos1, 0.f);
		}
		for (int32 i = 5; i < focusMonsterNumber; ++i)
		{
			focusMonsterLocation[i] = Vector3(XPos + XInterval * (i - 5), YPos2, 0.f);
		}

		//for (int32 i = 0; i < randomMonsterNumber; ++i)
		//{
		//	JobLoadPawn *jobLoadRMonster = new JobLoadPawn("Assets\\Data\\Monster2.lua", Transform(Vector3(100.f, 150.f, 0.f)));
		//	ASSERT(jobLoadRMonster);
		//	JobSystem::GetInstance()->RunJob(*jobLoadRMonster, "Default");
		//}

		for (int32 i = 0; i < focusMonsterNumber; ++i)
		{
			JobLoadPawn *jobLoadFMonster = new JobLoadPawn("Assets\\Data\\Monster1.lua", Transform(focusMonsterLocation[i]));
			ASSERT(jobLoadFMonster);
			JobSystem::GetInstance()->RunJob(*jobLoadFMonster, "Default");
		}

	
		bool bQuit = false;
		do
		{
			// We need to let GLib do it's thing. 
			GLib::Service(bQuit);

			//DEBUG_PRINT("FPS: %f", (1.0f / RealTimeManager::GetInstance()->GetLastFrameTimeS()));
			if (!bQuit)
			{
				engine.Run();
			}

			bQuit = bSuccess = InputManager::GetInstance()->GetState(static_cast<uint32>(Key::ESC));
		} while (bQuit == false);
		
		//GLib::Shutdown();
	}

	engine.EngineShutdown();

#if defined _DEBUG
	_CrtDumpMemoryLeaks();
#endif // _DEBUG

	return 0;
}





