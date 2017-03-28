#pragma once

class Engine
{
public:
	Engine();
	~Engine();

	void EngineInit();

	void Run();

	void EngineQuit();

private:
	void engineMemoryInit();
	void engineInstancceInit();

	void engineInstanceDestroy();
	void engineMemoryFree();

private:

};

