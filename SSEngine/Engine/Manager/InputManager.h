#pragma once

#include "Core\Memory\New.h"

enum class Key : uint32 {
	ESC = 0x1b,
	A = 0x41, B = 0x42, C = 0x43, D = 0x44, E = 0x45, F = 0x46, G = 0x47, 
	H = 0x48, I = 0x49, J = 0x4a, K = 0x4b, L = 0x4c, M = 0x4d, N = 0x4e, O = 0x4f, 
	P = 0x50, Q = 0x51, R = 0x52, S = 0x53, T = 0x54, U = 0x55, V = 0x56, W = 0x57,
	X = 0x58, Y = 0x59, Z = 0x5a
};

// singleton class
class InputManager
{
public:
	static FORCEINLINE InputManager *CreateInstance();
	static FORCEINLINE InputManager *GetInstance();
	static FORCEINLINE void DestroyInstance();

	FORCEINLINE void SetState(uint32 i_Key, bool i_Press);
	FORCEINLINE bool GetState(uint32 i_Key) const;

private:
	InputManager();
	FORCEINLINE InputManager(const InputManager &i_other) {}
	FORCEINLINE InputManager& operator=(const InputManager &i_other) {}


	static InputManager *globalInstance_;

private:
	bool keyState_[256];
};






// implement
FORCEINLINE InputManager *InputManager::CreateInstance()
{
	ASSERT(InputManager::globalInstance_ == nullptr);
	InputManager::globalInstance_ = new TRACK_NEW InputManager();
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


FORCEINLINE void InputManager::SetState(uint32 i_Key, bool i_Press)
{
	keyState_[i_Key] = i_Press;
}

FORCEINLINE bool InputManager::GetState(uint32 i_Key) const
{
	return keyState_[static_cast<uint8>(i_Key)];
}

