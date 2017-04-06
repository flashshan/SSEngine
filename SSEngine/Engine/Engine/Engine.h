#pragma once

class Engine
{
public:
	Engine();
	~Engine();

	void EngineStartup();

	void Run();

	void EngineShutdown();

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

