#include "Manager\InputManager.h"

InputManager *InputManager::globalInstance_ = nullptr;

InputManager::InputManager()
{
	for (int i = 0; i < 256; i++)
	{
		keyState_[i] = false;
	}
}