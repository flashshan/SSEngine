#include "Manager\WorldManager.h"

#include "Controller\FocusMoveController.h"
#include "Controller\RandomMoveController.h"
#include "Controller\ControllerManager.h"

#include "Core\Memory\New.h"
#include "Core\Basic\LuaData.h"
#include "Core\Basic\FileIO.h"

WorldManager *WorldManager::globalInstance_ = nullptr;

WorldManager::~WorldManager()
{
	pawns_.Clear();
	actors_.Clear();
	players_.Clear();
}


template<> WeakPtr<Actor> WorldManager::addActorToWorld<Actor>(void *i_actor)
{
	ASSERT(i_actor != nullptr);
	StrongPtr<Actor> strongActor = static_cast<Actor *>(i_actor);
	ASSERT(strongActor);

	EnterCriticalSection(&criticalSection_);
	newActors_.Push(strongActor);
	LeaveCriticalSection(&criticalSection_);
	return WeakPtr<Actor>(strongActor);
}

template<> WeakPtr<Pawn> WorldManager::addPawnToWorld<Pawn>(void *i_pawn)
{
	ASSERT(i_pawn != nullptr);
	StrongPtr<Pawn> strongPawn = static_cast<Pawn*>(i_pawn);
	ASSERT(strongPawn);

	HashedString type(strongPawn->GetType());
	if (type == typePlayer)
	{
		ControllerManager::GetInstance()->AddPlayerController(strongPawn);
		EnterCriticalSection(&criticalSection_);
		newPlayers_.Push(strongPawn);
		LeaveCriticalSection(&criticalSection_);
	}
	else
	{
		if (type == typeRandomMoveMonster)
		{
			StrongPtr<IController> newRandomMoveController = new TRACK_NEW RandomMoveController(WeakPtr<Pawn>(strongPawn));
			ASSERT(newRandomMoveController);
			strongPawn->SetController(newRandomMoveController);
			ControllerManager::GetInstance()->AddMonsterController(newRandomMoveController);
		}
		else if (type == typeFocusMoveMonster)
		{
			StrongPtr<IController> newFocusMoveController = new TRACK_NEW FocusMoveController(WeakPtr<Pawn>(strongPawn), GetPlayer(0));
			ASSERT(newFocusMoveController);
			strongPawn->SetController(newFocusMoveController);
			ControllerManager::GetInstance()->AddMonsterController(newFocusMoveController);
		}
		EnterCriticalSection(&criticalSection_);
		newPawns_.Push(strongPawn);
		LeaveCriticalSection(&criticalSection_);
	}
	return WeakPtr<Pawn>(strongPawn);
}

// add actors from queue to array, set active
void WorldManager::AddNewActorsIntoArray()
{
	while (!newActors_.Empty())
	{
		actors_.Add(newActors_.Front());
		actors_.Back()->SetActive(true);
		newActors_.Pop();
	}
	while (!newPlayers_.Empty())
	{
		players_.Add(newPlayers_.Front());
		players_.Back()->SetActive(true);
		newPlayers_.Pop();
	}
	while (!newPawns_.Empty())
	{
		pawns_.Add(newPawns_.Front());
		pawns_.Back()->SetActive(true);
		newPawns_.Pop();
	}
}

// spawn actor according to some build in types
template<> WeakPtr<Actor> WorldManager::SpawnActor<Actor>(const Transform &i_transform, const char *i_name, const char *i_type)
{
	Actor *actor = new Actor(i_transform, i_name, i_type);
	ASSERT(actor != nullptr);
	return addActorToWorld<Actor>(actor);
}

template<> WeakPtr<Pawn> WorldManager::SpawnPawn<Pawn>(const Transform &i_transform, const char *i_name, const char *i_type)
{
	Pawn *pawn = new Pawn(i_transform, i_name, i_type);
	ASSERT(pawn != nullptr);
	return addPawnToWorld<Pawn>(pawn);
}

// work on jobs
template<> WeakPtr<Actor> WorldManager::SpawnActorFromLua<Actor>(const char *i_luaFileName)
{
	lua_State * luaState = luaL_newstate();
	ASSERT(luaState);

	luaL_openlibs(luaState);

	size_t sizeFile = 0;
	uint8_t *pFileContents = LoadFile(i_luaFileName, sizeFile);

	Actor *actor = nullptr;
	if (pFileContents && sizeFile)
	{
		int	result = 0;
		// Do the initial buffer parsing
		result = luaL_loadbuffer(luaState, reinterpret_cast<char *>(pFileContents), sizeFile, nullptr);
		ASSERT(result == 0);
		lua_pcall(luaState, 0, 0, 0);
		ASSERT(result == 0);

		int	type = LUA_TNIL;

		result = lua_getglobal(luaState, "Actor");
		ASSERT(result == LUA_TTABLE);
		
		const char *actorName = nullptr;
		lua_pushstring(luaState, "name");
		type = lua_gettable(luaState, -2);
		ASSERT(type == LUA_TSTRING);
		actorName = lua_tostring(luaState, -1);
		ASSERT(actorName != nullptr);
		actorName = _strdup(actorName);
		lua_pop(luaState, 1);

		const char *actorType = nullptr;
		lua_pushstring(luaState, "type");
		type = lua_gettable(luaState, -2);
		ASSERT(type == LUA_TSTRING);
		actorType = lua_tostring(luaState, -1);
		ASSERT(actorType != nullptr);
		actorType = _strdup(actorType);
		lua_pop(luaState, 1);

		lua_pushstring(luaState, "transform");
		type = lua_gettable(luaState, -2);
		ASSERT(type == LUA_TTABLE);

		Transform actorTransform;
		lua_pushstring(luaState, "location");
		type = lua_gettable(luaState, -2);
		ASSERT(type == LUA_TTABLE);
		float location[3] = { 0.0f };
		ReadFloatArray(luaState, -1, location, 3);
		actorTransform.SetLocation(Vector3(location[0], location[1], location[2]));
		lua_pop(luaState, 1);

		lua_pushstring(luaState, "rotation");
		type = lua_gettable(luaState, -2);
		ASSERT(type == LUA_TTABLE);
		float rotation[3] = { 0.0f };
		ReadFloatArray(luaState, -1, rotation, 3);
		actorTransform.SetRotation(Vector3(rotation[0], rotation[1], rotation[2]));
		lua_pop(luaState, 1);

		lua_pushstring(luaState, "scale");
		type = lua_gettable(luaState, -2);
		ASSERT(type == LUA_TTABLE);
		float scale[3] = { 0.0f };
		ReadFloatArray(luaState, -1, scale, 3);
		actorTransform.SetScale(Vector3(scale[0], scale[1], scale[2]));
		lua_pop(luaState, 2);

		lua_pushstring(luaState, "canCollide");
		type = lua_gettable(luaState, -2);
		ASSERT(type == LUA_TBOOLEAN);
		lua_pushstring(luaState, "static");
		type = lua_gettable(luaState, -3);
		ASSERT(type == LUA_TBOOLEAN);
		int canCollide = lua_toboolean(luaState, -2);
		int tempStatic = lua_toboolean(luaState, -1);
		lua_pop(luaState, 2);

		actor = new TRACK_NEW Actor(actorTransform, actorName, actorType, (canCollide != 0), (tempStatic != 0));
		ASSERT(actor != nullptr);
		free(const_cast<char*>(actorName));
		free(const_cast<char*>(actorType));

		lua_pushstring(luaState, "boundingBox");
		type = lua_gettable(luaState, -2);
		ASSERT((type == LUA_TNIL) || (type == LUA_TTABLE));

		if (type == LUA_TTABLE)
		{
			Box2D actorBoundingBox;
			lua_pushstring(luaState, "center");
			type = lua_gettable(luaState, -2);
			ASSERT(type == LUA_TTABLE);
			float center[3] = { 0.0f };
			ReadFloatArray(luaState, -1, center, 3);
			actorBoundingBox.SetCenter(Vector2(center[0], center[1]));
			lua_pop(luaState, 1);

			lua_pushstring(luaState, "extend");
			type = lua_gettable(luaState, -2);
			ASSERT(type == LUA_TTABLE);
			float extend[3] = { 0.0f };
			ReadFloatArray(luaState, -1, extend, 3);
			actorBoundingBox.SetExtend(Vector2(extend[0], extend[1]));
			lua_pop(luaState, 1);
			actor->SetBoundingBox(actorBoundingBox);
		}
		lua_pop(luaState, 1);

		lua_pushstring(luaState, "renderObject");
		type = lua_gettable(luaState, -2);
		// It either needs to be a table or not there (no physics applied)
		ASSERT((type == LUA_TNIL) || (type == LUA_TTABLE));

		if (type == LUA_TTABLE)
		{
			lua_pushstring(luaState, "path");
			type = lua_gettable(luaState, -2);
			ASSERT(type == LUA_TSTRING);
			
			lua_pushstring(luaState, "priority");
			type = lua_gettable(luaState, -3);
			ASSERT(type == LUA_TNUMBER);

			const char *renderPath = lua_tostring(luaState, -2);
			uint32 priority = static_cast<uint32>(lua_tonumber(luaState, -1));
			actor->AddRenderObject(renderPath, priority);

			lua_pop(luaState, 3);
		}

		lua_pushstring(luaState, "physicsObject");
		type = lua_gettable(luaState, -2);
		ASSERT((type == LUA_TNIL) || (type == LUA_TTABLE));

		if (type == LUA_TTABLE)
		{
			lua_pushstring(luaState, "mass");
			type = lua_gettable(luaState, -2);
			ASSERT(type == LUA_TNUMBER);

			lua_pushstring(luaState, "drag");
			type = lua_gettable(luaState, -3);
			ASSERT(type == LUA_TNUMBER);

			float mass = static_cast<float>(lua_tonumber(luaState, -2));
			float drag = static_cast<float>(lua_tonumber(luaState, -1));
			actor->AddPhysicsObject(mass, drag);

			lua_pop(luaState, 2);
		}

		lua_pop(luaState, 2);

		int stackHeight = lua_gettop(luaState);
		ASSERT(stackHeight == 0);
	}

	delete[] pFileContents;
	lua_close(luaState);

	return addActorToWorld<Actor>(actor);
}

// work on jobs
template<> WeakPtr<Pawn> WorldManager::SpawnPawnFromLua<Pawn>(const char *i_luaFileName)
{
	lua_State * luaState = luaL_newstate();
	ASSERT(luaState);

	luaL_openlibs(luaState);

	size_t sizeFile = 0;
	uint8_t *pFileContents = LoadFile(i_luaFileName, sizeFile);

	Pawn *pawn = nullptr;
	if (pFileContents && sizeFile)
	{
		int	result = 0;
	
		result = luaL_loadbuffer(luaState, reinterpret_cast<char *>(pFileContents), sizeFile, nullptr);
		ASSERT(result == 0);
		lua_pcall(luaState, 0, 0, 0);
		ASSERT(result == 0);

		int	type = LUA_TNIL;

		result = lua_getglobal(luaState, "Pawn");
		ASSERT(result == LUA_TTABLE);

		const char *pawnName = nullptr;
		lua_pushstring(luaState, "name");
		type = lua_gettable(luaState, -2);
		ASSERT(type == LUA_TSTRING);
		pawnName = lua_tostring(luaState, -1);
		ASSERT(pawnName != nullptr);
		pawnName = _strdup(pawnName);
		lua_pop(luaState, 1);

		const char *pawnType = nullptr;
		lua_pushstring(luaState, "type");
		type = lua_gettable(luaState, -2);
		ASSERT(type == LUA_TSTRING);
		pawnType = lua_tostring(luaState, -1);
		ASSERT(pawnType != nullptr);
		pawnType = _strdup(pawnType);
		lua_pop(luaState, 1);

		lua_pushstring(luaState, "transform");
		type = lua_gettable(luaState, -2);
		ASSERT(type == LUA_TTABLE);

		Transform pawnTransform;
		lua_pushstring(luaState, "location");
		type = lua_gettable(luaState, -2);
		ASSERT(type == LUA_TTABLE);
		float location[3] = { 0.0f };
		ReadFloatArray(luaState, -1, location, 3);
		pawnTransform.SetLocation(Vector3(location[0], location[1], location[2]));
		lua_pop(luaState, 1);

		lua_pushstring(luaState, "rotation");
		type = lua_gettable(luaState, -2);
		ASSERT(type == LUA_TTABLE);
		float rotation[3] = { 0.0f };
		ReadFloatArray(luaState, -1, rotation, 3);
		pawnTransform.SetRotation(Vector3(rotation[0], rotation[1], rotation[2]));
		lua_pop(luaState, 1);

		lua_pushstring(luaState, "scale");
		type = lua_gettable(luaState, -2);
		ASSERT(type == LUA_TTABLE);
		float scale[3] = { 0.0f };
		ReadFloatArray(luaState, -1, scale, 3);
		pawnTransform.SetScale(Vector3(scale[0], scale[1], scale[2]));
		lua_pop(luaState, 2);

		lua_pushstring(luaState, "canCollide");
		type = lua_gettable(luaState, -2);
		ASSERT(type == LUA_TBOOLEAN);
		lua_pushstring(luaState, "static");
		type = lua_gettable(luaState, -3);
		ASSERT(type == LUA_TBOOLEAN);
		int canCollide = lua_toboolean(luaState, -2);
		int tempStatic = lua_toboolean(luaState, -1);
		lua_pop(luaState, 2);

		pawn = new TRACK_NEW Pawn(pawnTransform, pawnName, pawnType, (canCollide != 0), (tempStatic != 0));
		ASSERT(pawn != nullptr);
		free(const_cast<char*>(pawnName));
		free(const_cast<char*>(pawnType));

		lua_pushstring(luaState, "boundingBox");
		type = lua_gettable(luaState, -2);
		ASSERT((type == LUA_TNIL) || (type == LUA_TTABLE));

		if (type == LUA_TTABLE)
		{
			Box2D pawnBoundingBox;
			lua_pushstring(luaState, "center");
			type = lua_gettable(luaState, -2);
			ASSERT(type == LUA_TTABLE);
			float center[3] = { 0.0f };
			ReadFloatArray(luaState, -1, center, 3);
			pawnBoundingBox.SetCenter(Vector2(center[0], center[1]));
			lua_pop(luaState, 1);

			lua_pushstring(luaState, "extend");
			type = lua_gettable(luaState, -2);
			ASSERT(type == LUA_TTABLE);
			float extend[3] = { 0.0f };
			ReadFloatArray(luaState, -1, extend, 3);
			pawnBoundingBox.SetExtend(Vector2(extend[0], extend[1]));
			lua_pop(luaState, 1);

			pawn->SetBoundingBox(pawnBoundingBox);
		}
		lua_pop(luaState, 1);

		lua_pushstring(luaState, "renderObject");
		type = lua_gettable(luaState, -2);
		// It either needs to be a table or not there (no physics applied)
		ASSERT((type == LUA_TNIL) || (type == LUA_TTABLE));

		if (type == LUA_TTABLE)
		{
			lua_pushstring(luaState, "path");
			type = lua_gettable(luaState, -2);
			ASSERT(type == LUA_TSTRING);

			lua_pushstring(luaState, "priority");
			type = lua_gettable(luaState, -3);
			ASSERT(type == LUA_TNUMBER);

			const char *renderPath = lua_tostring(luaState, -2);
			uint32 priority = static_cast<uint32>(lua_tonumber(luaState, -1));
			pawn->AddRenderObject(renderPath, priority);

			lua_pop(luaState, 3);
		}

		lua_pushstring(luaState, "physicsObject");
		type = lua_gettable(luaState, -2);
		ASSERT((type == LUA_TNIL) || (type == LUA_TTABLE));

		if (type == LUA_TTABLE)
		{
			lua_pushstring(luaState, "mass");
			type = lua_gettable(luaState, -2);
			ASSERT(type == LUA_TNUMBER);

			lua_pushstring(luaState, "drag");
			type = lua_gettable(luaState, -3);
			ASSERT(type == LUA_TNUMBER);

			float mass = static_cast<float>(lua_tonumber(luaState, -2));
			float drag = static_cast<float>(lua_tonumber(luaState, -1));
			pawn->AddPhysicsObject(mass, drag);

			lua_pop(luaState, 2);
		}

		lua_pop(luaState, 2);

		int stackHeight = lua_gettop(luaState);
		ASSERT(stackHeight == 0);
	}

	delete[] pFileContents;
	lua_close(luaState);

	return addPawnToWorld<Pawn>(pawn);
}


void WorldManager::ActorsPreCalculation()
{
	size_t count = actors_.Size();
	for (size_t i = 0; i < count; ++i)
	{
		actors_[i]->PreCalculation();
	}
	count = players_.Size();
	for (size_t i = 0; i < count; ++i)
	{
		players_[i]->PreCalculation();
	}
	count = pawns_.Size();
	for (size_t i = 0; i < count; ++i)
	{
		pawns_[i]->PreCalculation();
	}
}

void WorldManager::ActorsEarlyUpdate()
{
	size_t count = actors_.Size();
	for (size_t i = 0; i < count; ++i)
	{
		actors_[i]->EarlyUpdate();
	}
	count = players_.Size();
	for (size_t i = 0; i < count; ++i)
	{
		players_[i]->EarlyUpdate();
	}
	count = pawns_.Size();
	for (size_t i = 0; i < count; ++i)
	{
		pawns_[i]->EarlyUpdate();
	}
}

void WorldManager::ActorsUpdate()
{
	size_t count = actors_.Size();
	for (size_t i = 0; i < count; ++i)
	{
		actors_[i]->Update();
	}
	count = players_.Size();
	for (size_t i = 0; i < count; ++i)
	{
		players_[i]->Update();
	}
	count = pawns_.Size();
	for (size_t i = 0; i < count; ++i)
	{
		pawns_[i]->Update();
	}
}

void WorldManager::ActorsActualUpdate()
{
	size_t count = actors_.Size();
	for (size_t i = 0; i < count; ++i)
	{
		actors_[i]->ActualUpdate();
	}
	count = players_.Size();
	for (size_t i = 0; i < count; ++i)
	{
		players_[i]->ActualUpdate();
	}
	count = pawns_.Size();
	for (size_t i = 0; i < count; ++i)
	{
		pawns_[i]->ActualUpdate();
	}
}

void WorldManager::ActorsLateUpdate()
{
	size_t count = actors_.Size();
	for (size_t i = 0; i < count; ++i)
	{
		actors_[i]->LateUpdate();
	}
	count = players_.Size();
	for (size_t i = 0; i < count; ++i)
	{
		players_[i]->LateUpdate();
	}
	count = pawns_.Size();
	for (size_t i = 0; i < count; ++i)
	{
		pawns_[i]->LateUpdate();
	}
}
