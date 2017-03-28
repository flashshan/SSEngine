#include "SubSystem\InputManager.h"


InputManager::InputManager()
{
	for (int i = 0; i < 256; i++)
	{
		keyState_[i] = false;
	}
}