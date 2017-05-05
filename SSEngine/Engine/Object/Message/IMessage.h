#pragma once

#include <Windows.h>
#include "Core\String\StringPool.h"

enum class MessageType {
	EUndefined,
	ECollision,
};


class IMessage
{
public:

	virtual	~IMessage() {}
	FORCEINLINE MessageType GetType() const { return type_; }

protected:
	MessageType type_;
};
