#include "Engine\Engine.h"

#include "Engine\SubSystem\InputManager.h"
#include "Engine\SubSystem\RenderManager.h"
#include "Manager\WorldManager.h"

Engine::Engine()
{
}

Engine::~Engine()
{
}

void Engine::EngineInit()
{
	engineMemoryInit();
	engineInstancceInit();
}

void Engine::Run()
{

}

void Engine::EngineQuit()
{
	engineInstanceDestroy();
	engineMemoryFree();
}



void Engine::engineMemoryInit()
{
}

void Engine::engineInstancceInit()
{

}

void Engine::engineInstanceDestroy()
{

}

void Engine::engineMemoryFree()
{

}