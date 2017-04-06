#pragma once

#include "Jobs\IJob.h"
#include "Core\CoreMinimal.h"


class JobLoadPawn : public IJob
{
public:
	FORCEINLINE JobLoadPawn(const char * i_luaFilePath);
	virtual void Run() override;

private:
	const char* luaFilePath_;
};


FORCEINLINE JobLoadPawn::JobLoadPawn(const char * i_luaFilePath)
	: IJob("Load Pawn From Lua"), luaFilePath_(i_luaFilePath)
{
	ASSERT(luaFilePath_);
}