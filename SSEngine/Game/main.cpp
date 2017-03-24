// main.cpp : Defines the entry point for the application.
#include <Windows.h>
#include <stdio.h>
#include <stdint.h>
#include <memory.h>

#include <conio.h>
#include <time.h>
#include <vector>

#include "Engine\SubSystem\InputManager.h"
#include "Engine\SubSystem\RenderManager.h"

#include "MonsterChase\Object\Player.h"
#include "MonsterChase\Object\Monster.h"

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

#define DEFAULT_MONSTER_NUMBER 10


int Monster::s_count;

int WINAPI wWinMain(HINSTANCE i_hInstance, HINSTANCE i_hPrevInstance, LPWSTR i_lpCmdLine, int i_nCmdShow)
{
	// first we need to initialize GLib
	bool bSuccess = GLib::Initialize(i_hInstance, i_nCmdShow, "MonsterChase", -1, 800, 600);

	if (bSuccess)
	{
		// calculate last frame time
		RealTimeManager::GetInstance()->CalcLastFrameTime_ms();

		// Set a callback for notification of key presses
		GLib::SetKeyStateChangeCallback(TestKeyCallback);

		srand(static_cast<unsigned int>(time(0)));

		// assign player
		StrongPtr<Player> player = new Player("Player1");
		SLOW_ASSERT(player, ErrorMessage::kNullPointer);

		StrongPtr<GameObject> playerPtr = player->GetController()->GetGameObject();
		RenderManager::GetInstance()->AddToRenderList(new RenderObject(playerPtr, "data\\player.dds"));

		// assign monsters
		int monsterNumber = DEFAULT_MONSTER_NUMBER;

		Monster::s_count = 0;

		std::vector<StrongPtr<Monster>> monsters;
		monsters.reserve(monsterNumber);

		std::vector<StrongPtr<GameObject>> monstersPtr;
		monstersPtr.reserve(monsterNumber);

		StrongPtr<Monster> tempMonster;
		for (int i = 0; i < monsterNumber; i++)
		{
#if defined RANDOM_MOVE_MONSTER
			tempMonster = new Monster();
#elif defined FOCUS_MOVE_MONSTER 
			tempMonster = new Monster(Monster::FocusMoveMonster, player->GetController()->GetGameObject());
#endif // 
			SLOW_ASSERT(tempMonster, ErrorMessage::kNullPointer);
			monsters.push_back(tempMonster);

			monstersPtr.push_back(StrongPtr<GameObject>(tempMonster->GetController()->GetGameObject()));
			RenderManager::GetInstance()->AddToRenderList(new RenderObject(monstersPtr.back(), "data\\monster.dds"));
		}

		bool bQuit = false;

		do
		{
			// We need to let GLib do it's thing. 
			GLib::Service(bQuit);

			DEBUG_PRINT("FPS: %f", (1.0f / RealTimeManager::GetInstance()->GetLastFrameTimeS()));

			if (!bQuit)
			{
				//printf_s("\nPress A to move left, D to move right, W to move up, S to move down, M to generate monster, R to remove the first monster, or Q to quit.\n");
				player->Move();
				player->Update();

				for (uint32 i = 0; i < monsters.size(); i++)
				{
					monsters[i]->Update();
				}

				//render part
				RenderManager::GetInstance()->RenderUpdate();
			}

			bQuit = bSuccess = InputManager::GetInstance()->GetState(Key::ESC);
		} while (bQuit == false);
		
		//GLib::Shutdown();
	}

#if defined _DEBUG
	_CrtDumpMemoryLeaks();
#endif // _DEBUG

	return 0;
}





