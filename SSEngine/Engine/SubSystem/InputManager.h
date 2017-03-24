#pragma once

#include "Core\CoreMinimal.h"

namespace Key {
	enum KeyName {
		ESC = 0x1b,
		A = 0x41,
		D = 0x44,
		S = 0x53,
		W = 0x57,
	};
};

class InputManager
{
public:
	static FORCEINLINE InputManager *GetInstance();

	FORCEINLINE void SetState(const unsigned int i_KeyID, const bool i_Press);
	FORCEINLINE bool GetState(const unsigned int i_KeyID) const;

private:
	FORCEINLINE InputManager();

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

FORCEINLINE void InputManager::SetState(const unsigned int i_KeyID, const bool i_Press)
{
	keyState_[i_KeyID] = i_Press;
}

FORCEINLINE bool InputManager::GetState(const unsigned int i_KeyID) const
{
	return keyState_[i_KeyID];
}

FORCEINLINE InputManager::InputManager()
{
	for (int i = 0; i < 256; i++)
		keyState_[i] = false;
}