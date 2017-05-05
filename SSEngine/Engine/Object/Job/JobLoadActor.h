#pragma once

#include "IJob.h"
#include "Core\CoreMinimal.h"
#include "Component\Transform.h"

class JobLoadActor : public IJob
{
public:
	FORCEINLINE JobLoadActor(const char * i_luaFilePath, const Transform &i_transform);
	virtual void Run() override;

private:
	const char* luaFilePath_;
	Transform spawnTransform_;
};


FORCEINLINE JobLoadActor::JobLoadActor(const char * i_luaFilePath, const Transform &i_transform)
	: IJob("Load Actor From Lua"), luaFilePath_(i_luaFilePath), spawnTransform_(i_transform)
{
	ASSERT(luaFilePath_);
}