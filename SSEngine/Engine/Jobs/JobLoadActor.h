#pragma once

#include "Jobs\IJob.h"
#include "Core\CoreMinimal.h"


class JobLoadActor : public IJob
{
public:
	FORCEINLINE JobLoadActor(const char * i_luaFilePath);
	virtual void Run() override;

private:
	const char* luaFilePath_;
};


FORCEINLINE JobLoadActor::JobLoadActor(const char * i_luaFilePath)
	: IJob("Load Actor From Lua"), luaFilePath_(i_luaFilePath)
{
	ASSERT(luaFilePath_);
}