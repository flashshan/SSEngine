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
	pawns_.clear();
	actors_.clear();
	players_.clear();
}


template<> WeakPtr<Actor> WorldManager::addActorToWorld<Actor>(void *i_actor)
{
	if (i_actor == nullptr) return WeakPtr<Actor>();

	StrongPtr<Actor> strongActor = static_cast<Actor *>(i_actor);
	actors_.push_back(strongActor);

	return WeakPtr<Actor>(strongActor);
}

template<> WeakPtr<Pawn> WorldManager::addPawnToWorld<Pawn>(void *i_pawn)
{
	if (i_pawn == nullptr) return WeakPtr<Pawn>();

	StrongPtr<Pawn> strongPawn = static_cast<Pawn*>(i_pawn);
	HashedString type(strongPawn->GetType());
	if (type == typePlayer)
	{
		ControllerManager::GetInstance()->AddPlayerController(strongPawn);
		players_.push_back(strongPawn);
	}
	else if (type == typeRandomMoveMonster)
	{
		StrongPtr<IController> newRandomMoveController = new RandomMoveController(strongPawn);
		strongPawn->SetController(newRandomMoveController);
		ControllerManager::GetInstance()->AddMonsterController(newRandomMoveController);
		pawns_.push_back(strongPawn);
	}
	else if (type == typeFocusMoveMonster)
	{
		StrongPtr<IController> newFocusMoveController = new FocusMoveController(strongPawn, GetPlayer(0));
		strongPawn->SetController(newFocusMoveController);
		ControllerManager::GetInstance()->AddMonsterController(newFocusMoveController);
		pawns_.push_back(strongPawn);
	}
	else
	{
		// No controller
		pawns_.push_back(strongPawn);
	}
	return WeakPtr<Pawn>(strongPawn);
}

// spawn actor according to some build in types
template<> WeakPtr<Actor> WorldManager::SpawnActor<Actor>(const Transform &i_transform, const char *i_name, const char *i_type)
{
	Actor *actor = new Actor(i_transform, i_name, i_type);
	return addActorToWorld<Actor>(actor);
}

template<> WeakPtr<Pawn> WorldManager::SpawnPawn<Pawn>(const Transform &i_transform, const char *i_name, const char *i_type)
{
	Pawn *pawn = new Pawn(i_transform, i_name, i_type);
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
		const char *actorName = nullptr;
		const char *actorType = nullptr;
		Transform actorTransform;
		Box2D actorBoundingBox;

		int	result = 0;
		// Do the initial buffer parsing
		result = luaL_loadbuffer(luaState, reinterpret_cast<char *>(pFileContents), sizeFile, nullptr);
		ASSERT(result == 0);
		lua_pcall(luaState, 0, 0, 0);
		ASSERT(result == 0);

		int	type = LUA_TNIL;

		result = lua_getglobal(luaState, "Actor");
		ASSERT(result == LUA_TTABLE);

		lua_pushstring(luaState, "name");
		type = lua_gettable(luaState, -2);
		ASSERT(type == LUA_TSTRING);
		actorName = lua_tostring(luaState, -1);
		ASSERT(actorName != nullptr);
		actorName = _strdup(actorName);
		lua_pop(luaState, 1);

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

		lua_pushstring(luaState, "boundingBox");
		type = lua_gettable(luaState, -2);
		ASSERT(type == LUA_TTABLE);

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
		lua_pop(luaState, 2);

		actor = new Actor(actorTransform, actorName, actorType);
		actor->SetBoundingBox(actorBoundingBox);
		free(const_cast<char*>(actorName));
		free(const_cast<char*>(actorType));

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
			int priority = static_cast<int>(lua_tonumber(luaState, -1));
			actor->AddRenderObject(renderPath);

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
		const char *pawnName = nullptr;
		const char *pawnType = nullptr;
		Transform pawnTransform;
		Box2D pawnBoundingBox;

		int	result = 0;
	
		result = luaL_loadbuffer(luaState, reinterpret_cast<char *>(pFileContents), sizeFile, nullptr);
		ASSERT(result == 0);
		lua_pcall(luaState, 0, 0, 0);
		ASSERT(result == 0);

		int	type = LUA_TNIL;

		result = lua_getglobal(luaState, "Pawn");
		ASSERT(result == LUA_TTABLE);

		lua_pushstring(luaState, "name");
		type = lua_gettable(luaState, -2);
		ASSERT(type == LUA_TSTRING);
		pawnName = lua_tostring(luaState, -1);
		ASSERT(pawnName != nullptr);
		pawnName = _strdup(pawnName);
		lua_pop(luaState, 1);

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

		lua_pushstring(luaState, "boundingBox");
		type = lua_gettable(luaState, -2);
		ASSERT(type == LUA_TTABLE);

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
		lua_pop(luaState, 2);

		pawn = new Pawn(pawnTransform, pawnName, pawnType);
		pawn->SetBoundingBox(pawnBoundingBox);
		free(const_cast<char*>(pawnName));
		free(const_cast<char*>(pawnType));

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
			int priority = static_cast<int>(lua_tonumber(luaState, -1));
			pawn->AddRenderObject(renderPath);

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


void WorldManager::EarlyUpdate()
{
	for (auto && x : actors_)
	{
		x->EarlyUpdate();
	}
	for (auto && x : players_)
	{
		x->EarlyUpdate();
	}
	for (auto && x : pawns_)
	{
		x->EarlyUpdate();
	}
}

void WorldManager::Update()
{
	for (auto && x : actors_)
	{
		x->Update();
	}
	for (auto && x : players_)
	{
		x->Update();
	}
	for (auto && x : pawns_)
	{
		x->Update();
	}
}

void WorldManager::LateUpdate()
{
	for (auto && x : actors_)
	{
		x->LateUpdate();
	}
	for (auto && x : players_)
	{
		x->LateUpdate();
	}
	for (auto && x : pawns_)
	{
		x->LateUpdate();
	}
}


