#pragma once

#include "Core\CoreMinimal.h"
#include "Core\Memory\New.h"

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
	static FORCEINLINE InputManager *CreateInstance();
	static FORCEINLINE InputManager *GetInstance();
	static FORCEINLINE void DestroyInstance();

	FORCEINLINE void SetState(const uint32 i_Key, const bool i_Press);
	FORCEINLINE bool GetState(const uint32 i_Key) const;

private:
	InputManager();
	FORCEINLINE InputManager(const InputManager &i_other) {}

	static InputManager *globalInstance_;

private:
	bool keyState_[256];
};






// implement
FORCEINLINE InputManager *InputManager::CreateInstance()
{
	ASSERT(InputManager::globalInstance_ == nullptr);
	InputManager::globalInstance_ = new InputManager();
	return InputManager::globalInstance_;
}

FORCEINLINE InputManager *InputManager::GetInstance()
{
	ASSERT(InputManager::globalInstance_ != nullptr);
	return InputManager::globalInstance_;
}

FORCEINLINE void InputManager::DestroyInstance()
{
	ASSERT(InputManager::globalInstance_ != nullptr);
	delete InputManager::globalInstance_;
	InputManager::globalInstance_ = nullptr;
}


FORCEINLINE void InputManager::SetState(const uint32 i_Key, const bool i_Press)
{
	keyState_[i_Key] = i_Press;
}

FORCEINLINE bool InputManager::GetState(const uint32 i_Key) const
{
	return keyState_[static_cast<uint8>(i_Key)];
}

