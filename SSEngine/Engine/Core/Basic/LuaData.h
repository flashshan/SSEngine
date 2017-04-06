#pragma once

#include "Lua\lua.hpp"

#include "ASSERT.h"


struct Vector3;

inline int GetLuaTableEntries(lua_State * i_pState, int i_index);
inline Vector3 GetVector3(lua_State * i_pState, int i_index);

size_t ReadIntArray(lua_State * i_pState, int i_index, int * o_pInts, size_t i_numInts);
size_t ReadFloatArray(lua_State * i_pState, int i_index, float * o_pFloats, size_t i_numFloats);
size_t GetFloatArray(lua_State * i_pState, int i_indexTable, const char * i_pArrayKey, float * o_pFloats, size_t i_numFloats);


inline int GetLuaTableEntries(lua_State * i_pState, int i_index)
{
	ASSERT(i_pState);
	ASSERT(lua_gettop(i_pState) >= -i_index);
	ASSERT(lua_type(i_pState, i_index) == LUA_TTABLE);

	lua_len(i_pState, i_index);
	ASSERT(lua_type(i_pState, -1) == LUA_TNUMBER);

	lua_Integer table_entries = lua_tointeger(i_pState, -1);
	lua_pop(i_pState, 1);

	ASSERT(table_entries <= INT_MAX);

	return static_cast<int>(table_entries);
}