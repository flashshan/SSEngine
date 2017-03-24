#include "Object\Player.h"
#include "Engine\Controller\PlayerController.h"
#include "Object\Monster.h"

#include <conio.h>
#include <time.h>
#include <stdlib.h>
#include <memory.h>
#include <stdio.h>

#include <crtdbg.h>
#include <vector>

#include "Engine/Core/CoreMinimal.h"

//#define RANDOM_MOVE_MONSTER
#define FOCUS_MOVE_MONSTER

// check if the input is a number
bool isLegalNumber(char* str)
{
	ASSERT(str != nullptr);
	if (sizeof(str) > 10) return false;
	for (int i = 0; str[i] != '\0';i++)
	{
		if (str[i] <'0' || str[i] >'9')
			return false;
	}
	return true;
}

// show information of player and monsters
void Display(Player *player, std::vector<Monster *> monsters)
{
	ASSERT(player != nullptr);
	Vector3 playerPosition = player->GetPosition();
	printf_s("\nPlayer %s at [%f,%f,%f].\n", player->GetName(), playerPosition.X, playerPosition.Y, playerPosition.Z);

	Vector3 monsterPosition;
	for (unsigned int i = 0; i < monsters.size(); i++)
	{
		monsterPosition = monsters[i]->GetPosition();
		printf_s("Monster %s at [%f,%f,%f].\n", monsters[i]->GetName(), monsterPosition.X, monsterPosition.Y, monsterPosition.Z);
	}
}

int Monster::s_count;

bool GameMainLoop()
{
	srand(static_cast<unsigned int>(time(0)));

	// assign player
	char playerName[DEFAULT_CHAR_LENGTH];
	printf_s("What would you like to name your player:\n");
	scanf_s("%s", playerName, DEFAULT_CHAR_LENGTH);

	Player *player = new Player(playerName);
	player = nullptr;
	//SLOW_ASSERT(player != nullptr, ErrorMessage::kNullPointer);

	// assign monsters
	int monsterNumber;
	printf_s("Notice: In the game, a sequential name was assigned to monsters.\n");
	printf_s("How many monsters to start:\n");

	char numberInput[DEFAULT_CHAR_LENGTH];
	scanf_s("%s", numberInput, DEFAULT_CHAR_LENGTH);
	ASSERT(isLegalNumber(numberInput));
	monsterNumber = atoi(numberInput);

	Monster::s_count = 0;

	std::vector<Monster *> monsters;
	monsters.reserve(monsterNumber);

	Monster *tempMonster;
	for (int i = 0; i < monsterNumber; i++)
	{
#if defined RANDOM_MOVE_MONSTER
		tempMonster = new Monster();
#elif defined FOCUS_MOVE_MONSTER 
		tempMonster = new Monster(Monster::FocusMoveMonster, player->GetController()->GetGameObject());
#endif // 
		monsters.push_back(tempMonster);
	}

	// first display information
	Display(player, monsters);

	// handle operations
	char commond;
	printf_s("\nPress A to move left, D to move right, W to move up, S to move down, M to generate monster, R to remove the first monster, or Q to quit.\n");

	while (true)
	{
		commond = _getch();
		printf_s("%c\n", commond);
		if (commond == 'Q' || commond == 'q')
			break;
		else if (commond == 'M' || commond == 'm')
		{
#if defined RANDOM_MOVE_MONSTER
			tempMonster = new Monster();
#elif defined FOCUS_MOVE_MONSTER 
			tempMonster = new Monster(Monster::FocusMoveMonster, player->GetController()->GetGameObject());
#endif // 
			monsters.push_back(tempMonster);

			Vector3 monsterPosition = tempMonster->GetPosition();
			printf_s("\nA new monster named %s generated at [%f,%f,%f].\n", tempMonster->GetName(), monsterPosition.X, monsterPosition.Y, monsterPosition.Z);
		}
		else if (commond == 'R' || commond == 'r')
		{
			std::swap(monsters[0], monsters.back());
			tempMonster = monsters.back();
			monsters.pop_back();
			Vector3 monsterPosition = tempMonster->GetPosition();
			printf_s("\nThe first monster named %s at [%f,%f,%f] was removed.\n", tempMonster->GetName(), monsterPosition.X, monsterPosition.Y, monsterPosition.Z);
			delete tempMonster;
		}
		else if (commond == 'W' || commond == 'w' || commond == 'A' || commond == 'a' || commond == 'S' || commond == 's' || commond == 'D' || commond == 'd')
		{
			// move monsters and player
			player->Move();
			player->Update();

			for (unsigned int i = 0;i < monsters.size();i++)
			{
				monsters[i]->Update();
			}

			// display information
			Display(player, monsters);
		}
		printf_s("\nPress A to move left, D to move right, W to move up, S to move down, M to generate monster or Q to quit.\n");
	}

	// delete all objects
	delete player;
	Monster *temp;
	while (!monsters.empty())
	{
		temp = monsters.back();
		monsters.pop_back();
		delete temp;
	}

	return true;
}


int main(int i_argc, char ** i_argl)
{
	GameMainLoop();

#if defined(_DEBUG)
	_CrtDumpMemoryLeaks();
#endif // _DEBUG

	return 0;
}