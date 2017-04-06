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
	void engineSubsystemInit();
	void engineManagerInit();

	void engineManagerDestroy();
	void engineSubsystemDestroy();
	void engineMemoryFree();

private:
	void *memoryBase_;
	size_t memorySize_;
	size_t stringPoolSize_;
};

