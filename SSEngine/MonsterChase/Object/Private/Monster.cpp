#include "Object\Monster.h"

#include <string.h>

Monster::Monster(MonsterType i_Type, StrongPtr<GameObject> i_FocusObject)
{
	char totalName[DEFAULT_CHAR_LENGTH];
	strcpy_s(totalName, "Monster");

	char strNum[10];
	_itoa_s(s_count, strNum, 10);
	strcat_s(totalName, strNum);

	name_ = _strdup(totalName);

	if (i_Type == RandomMoveMonster)
	{
		monsterController_ = new RandomMoveMonsterController();
	}
	else if(i_Type == FocusMoveMonster)
	{
		monsterController_ = new FocusMoveMonsterController();
		dynamic_cast<FocusMoveMonsterController *>(monsterController_)->setFocusObject(i_FocusObject);
	}

	s_count++;
}

Monster::Monster(const char *i_Name, MonsterType i_Type, StrongPtr<GameObject> i_FocusObject)
{
	if(i_Name)
		name_ = _strdup(i_Name);
	else
	{
		char totalName[DEFAULT_CHAR_LENGTH];
		strcpy_s(totalName, "Monster");

		char strNum[10];
		_itoa_s(s_count, strNum, 10);
		strcat_s(totalName, strNum);

		name_ = _strdup(totalName);
	}

	if (i_Type == RandomMoveMonster)
	{
		monsterController_ = new RandomMoveMonsterController();
	}
	else if (i_Type == FocusMoveMonster)
	{
		monsterController_ = new FocusMoveMonsterController();
		dynamic_cast<FocusMoveMonsterController *>(monsterController_)->setFocusObject(i_FocusObject);
	}

	s_count++;
}

Monster::Monster(const char *i_Name, const Vector3 &i_Position, MonsterType i_Type, StrongPtr<GameObject> i_FocusObject)
{
	if (i_Name)
		name_ = _strdup(i_Name);
	else
	{
		char totalName[DEFAULT_CHAR_LENGTH];
		strcpy_s(totalName, "Monster");

		char strNum[10];
		_itoa_s(s_count, strNum, 10);
		strcat_s(totalName, strNum);

		name_ = _strdup(totalName);
	}

	if (i_Type == RandomMoveMonster)
	{
		monsterController_ = new RandomMoveMonsterController();
	}
	else if (i_Type == FocusMoveMonster)
	{
		monsterController_ = new FocusMoveMonsterController();
		dynamic_cast<FocusMoveMonsterController *>(monsterController_)->setFocusObject(i_FocusObject);
	}

	s_count++;
}

Monster::~Monster()
{
	if (monsterController_)
		delete monsterController_;
	if (name_)
		free(name_);

	//s_count--;
}