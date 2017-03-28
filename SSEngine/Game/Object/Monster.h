//#pragma once
//
//#include "Engine\Core\Math\Vector3.h"
//#include "Engine\Controller\IGameObjectController.h"
//
//#include "FocusMoveMonsterController.h"
//#include "RandomMoveMonsterController.h"
//
//#define DEFAULT_CHAR_LENGTH 50
//
//class Monster
//{
//public:
//	static int s_count;
//	
//	enum MonsterType
//	{
//		RandomMoveMonster = 0,
//		FocusMoveMonster = 1,
//	};
//
//public:
//	explicit Monster(MonsterType i_Type = RandomMoveMonster, StrongPtr<GameObject> i_FocusObject = nullptr);
//	explicit Monster(const char *i_Name, MonsterType i_Type = RandomMoveMonster, StrongPtr<GameObject> i_FocusObject = nullptr);
//	explicit Monster(const char *i_Name, const Vector3 &i_Position, MonsterType i_Type = RandomMoveMonster, StrongPtr<GameObject> i_FocusObject = nullptr);
//	~Monster();
//
//	FORCEINLINE Monster(const Monster & i_Monster);
//	FORCEINLINE Monster(Monster && i_Monster);
//	FORCEINLINE Monster & operator= (const Monster &i_Monster);
//	FORCEINLINE Monster & operator= (Monster &&i_Monster);
//
//	FORCEINLINE const char* GetName() const { return name_; }
//	FORCEINLINE IGameObjectController *GetController() const { return monsterController_; }
//
//	FORCEINLINE Vector3 GetPosition() const { return monsterController_->GetPosition(); }
//	FORCEINLINE void Update() const;
//	
//private:
//	char *name_;
//	IGameObjectController *monsterController_;
//};
//
//
//
//
//
//
//
//
//// implement forceinline
//
//FORCEINLINE Monster::Monster(const Monster & i_Monster) : name_(_strdup(i_Monster.name_ ? i_Monster.name_ : "Monster"))
//{
//	if (dynamic_cast<RandomMoveMonsterController *>(i_Monster.monsterController_) != nullptr)
//		monsterController_ = new RandomMoveMonsterController(*dynamic_cast<RandomMoveMonsterController *>(i_Monster.monsterController_));
//	if (dynamic_cast<FocusMoveMonsterController *>(i_Monster.monsterController_) != nullptr)
//		monsterController_ = new FocusMoveMonsterController(*dynamic_cast<FocusMoveMonsterController *>(i_Monster.monsterController_));
//}
//FORCEINLINE Monster::Monster(Monster && i_Monster) : name_(i_Monster.name_), monsterController_(i_Monster.monsterController_)
//{
//	i_Monster.name_ = nullptr;
//	i_Monster.monsterController_ = nullptr;
//}
//
//FORCEINLINE Monster & Monster::operator= (const Monster &i_Monster)
//{
//	if (name_)
//		free(name_);
//	name_ = _strdup(i_Monster.name_ ? i_Monster.name_ : "Monster");
//	if (monsterController_)
//		delete monsterController_;
//	if (dynamic_cast<RandomMoveMonsterController *>(i_Monster.monsterController_) != nullptr)
//		monsterController_ = new RandomMoveMonsterController(*dynamic_cast<RandomMoveMonsterController *>(i_Monster.monsterController_));
//	if (dynamic_cast<FocusMoveMonsterController *>(i_Monster.monsterController_) != nullptr)
//		monsterController_ = new FocusMoveMonsterController(*dynamic_cast<FocusMoveMonsterController *>(i_Monster.monsterController_));
//}
//
//FORCEINLINE Monster & Monster::operator= (Monster &&i_Monster)
//{
//	std::swap(name_, i_Monster.name_);
//	std::swap(monsterController_, i_Monster.monsterController_);
//}
//
//FORCEINLINE void Monster::Update() const
//{
//	monsterController_->UpdateGameObject();
//};
//
//
