#pragma once

#include "Core\CoreMinimal.h"


enum class Key : uint32 {
	ESC = 0x1b,
	A = 0x41,
	D = 0x44,
	S = 0x53,
	W = 0x57,
};

class InputManager
{
public:
	static FORCEINLINE InputManager *GetInstance();

	FORCEINLINE void SetState(const uint32 i_Key, const bool i_Press);
	FORCEINLINE bool GetState(const uint32 i_Key) const;

private:
	InputManager();

private:
	bool keyState_[256];
};






// implement

FORCEINLINE InputManager *InputManager::GetInstance()
{
	static InputManager *globalInstance;
	if (globalInstance == nullptr)
	{
		globalInstance = new InputManager();
	}
	return globalInstance;
}


FORCEINLINE void InputManager::SetState(const uint32 i_Key, const bool i_Press)
{
	keyState_[i_Key] = i_Press;
}

FORCEINLINE bool InputManager::GetState(const uint32 i_Key) const
{
	return keyState_[static_cast<uint8>(i_Key)];
}

