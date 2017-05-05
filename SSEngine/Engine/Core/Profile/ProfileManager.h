#pragma once

#include <windows.h>

#include "Core\CoreMinimal.h"
#include "Core\String\HashedString.h"
#include "Core\Memory\New.h"



class Accumulator;

class ProfilerManager
{
public:
	static FORCEINLINE ProfilerManager* CreateInstance();
	static FORCEINLINE ProfilerManager* GetInstance();
	static FORCEINLINE void DestroyInstance();

	static FORCEINLINE Accumulator* GetAccumulator(const char *i_name);

	FORCEINLINE void AddAccumulator(const char * i_name, Accumulator *i_cccumulator);
private:
	FORCEINLINE ProfilerManager();
	FORCEINLINE ProfilerManager(const ProfilerManager &i_other) {};
	FORCEINLINE ProfilerManager& operator=(const ProfilerManager &i_other) {}

	static ProfilerManager * globalInstance_;

	std::map<HashedString, Accumulator *> accumulators_;
	CRITICAL_SECTION criticalSection_;
};





FORCEINLINE ProfilerManager* ProfilerManager::CreateInstance()
{
	ASSERT(ProfilerManager::globalInstance_ == nullptr);
	ProfilerManager::globalInstance_ = new TRACK_NEW ProfilerManager();
	return ProfilerManager::globalInstance_;
}

FORCEINLINE ProfilerManager* ProfilerManager::GetInstance()
{
	ASSERT(ProfilerManager::globalInstance_ != nullptr);
	return ProfilerManager::globalInstance_;
}

FORCEINLINE void ProfilerManager::DestroyInstance()
{
	ASSERT(ProfilerManager::globalInstance_ != nullptr);
	delete ProfilerManager::globalInstance_;
}



//
//PROFILE_UNSCOPED_UNGROUPED("Blah");
//Blah();

//PROFILE_UNSCOPED_UNPARENTED(¡°Orphan");





#define CONCAT_HELPER(left, right) left##right
#define CONCAT(left, right) CONCAT_HELPER(left, right)

#ifdef _DEBUG
#define ENABLE_PROFILING
#endif // _DEBUG


#if defined( ENABLE_PROFILING )

#define PROFILE_UNSCOPED(functionName)		\
   static Accumulator CONCAT(__Accumulator,__LINE__)(functionName);  ScopedTimer CONCAT(__Timer,__LINE__)(CONCAT(__Accumulator,__LINE__));

#define PROFILE_SCOPE_BEGIN(functionName)   {	\
   static Accumulator CONCAT(__Accumulator,__LINE__)(functionName);  ScopedTimer CONCAT(__Timer, __LINE__)(CONCAT(__Accumulator, __LINE__));
#define PROFILE_SCOPE_END()	   }


#define PROFILE_GROUP_UNSCOPED(systemName)		\
	ScopedTimer CONCAT(__Timer,__LINE__)(ProfilerManager::GetAccumulator(systemName));

#define PROFILE_GROUP_SCOPE_BEGIN(systemName)   {   \
	ScopedTimer CONCAT(__Timer,__LINE__)(ProfilerManager::GetAccumulator(systemName));
#define PROFILE_GROUP_SCOPE_END()    }



#else // ENABLE_PROFILING not defined

#define PROFILE_UNSCOPED(functionName)	NOOP;

#define PROFILE_SCOPE_BEGIN(functionName)	NOOP;
#define PROFILE_SCOPE_END()   NOOP;


#define PROFILE_GROUP_UNSCOPED(systemName)	NOOP;

#define PROFILE_GROUP_SCOPE_BEGIN(systemName)  NOOP;
#define PROFILE_GROUP_SCOPE_END()   NOOP;

#endif // ENABLE_PROFILING
