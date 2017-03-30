// main.cpp : Defines the entry point for the application.
#include <Windows.h>
#include <stdio.h>
#include <stdint.h>
#include <memory.h>

#include <conio.h>
#include <time.h>
#include <vector>

#include "Engine\Manager\InputManager.h"
#include "Engine\Manager\RealTimeManager.h"

#include "Manager\WorldManager.h"

#include "Engine\Engine.h"
//#if defined _DEBUG
//#define _CRTDBG_MAP_ALLOC
//#include <crtdbg.h>
//#endif // _DEBUG
#include "Glib\GLib.h"

void TestKeyCallback(unsigned int i_VKeyID, bool bWentDown)
{
#ifdef _DEBUG
	const size_t	lenBuffer = 65;
	char			Buffer[lenBuffer];

	sprintf_s(Buffer, lenBuffer, "VKey 0x%04x went %s\n", i_VKeyID, bWentDown ? "down" : "up");
	DEBUG_PRINT("%s", Buffer);
#endif // __DEBUG

	InputManager::GetInstance()->SetState(i_VKeyID, bWentDown);
}


//#define RANDOM_MOVE_MONSTER
#define FOCUS_MOVE_MONSTER

#define FOCUS_MONSTER_NUMBER 10
#define RANDOM_MONSTER_NUMBER 10

int WINAPI wWinMain(HINSTANCE i_hInstance, HINSTANCE i_hPrevInstance, LPWSTR i_lpCmdLine, int i_nCmdShow)
{
	Engine engine;
	engine.EngineInit();

	// first we need to initialize GLib
	bool bSuccess = GLib::Initialize(i_hInstance, i_nCmdShow, "MonsterChase", -1, 800, 600);

	const float ScreenSize = 100;

	if (bSuccess)
	{
		// calculate last frame time
		RealTimeManager::GetInstance()->CalcLastFrameTime_ms();

		// Set a callback for notification of key presses
		GLib::SetKeyStateChangeCallback(TestKeyCallback);

		srand(static_cast<unsigned int>(time(0)));

		// assign player
		Pawn* player = WorldManager::GetInstance()->SpawnPawn<Pawn>(Transform(Vector3(0.0f, 0.0f, 0.0f)), "Player1", "Player");
		SLOW_ASSERT(player, ErrorType::ENullPointer);
		player->AddRenderObject("data\\player.dds");
		player->AddPhysicsObject(100.0f);

		// assign monsters
		int32 focusMonsterNumber = FOCUS_MONSTER_NUMBER;

		Pawn* tempMonster;
		for (int i = 0; i < focusMonsterNumber; i++)
		{
			tempMonster = WorldManager::GetInstance()->SpawnPawn<Pawn>(Transform(Vector3(Vector2::RandomNormal() * ScreenSize)), "FMonster", "FocusMoveMonster");
			SLOW_ASSERT(tempMonster, ErrorType::ENullPointer);
			tempMonster->AddRenderObject("data\\focusMonster.dds");
		}

		int32 randomMonsterNumber = RANDOM_MONSTER_NUMBER;

		for (int i = 0; i < randomMonsterNumber; i++)
		{
			tempMonster = WorldManager::GetInstance()->SpawnPawn<Pawn>(Transform(Vector3(Vector2::RandomNormal() * ScreenSize)), "RMonster", "RandomMoveMonster");
			SLOW_ASSERT(tempMonster, ErrorType::ENullPointer);
			tempMonster->AddRenderObject("data\\randomMonster.dds");
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

	engine.EngineQuit();

#if defined _DEBUG
	_CrtDumpMemoryLeaks();
#endif // _DEBUG

	return 0;
}





