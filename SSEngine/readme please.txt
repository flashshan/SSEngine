V1.00
A 2D version of MonsterChase, hope you like it.
The entrance is in Game/main.cpp. I made MonsterChase project as a static library of the Game project.
10 monster were created in the beginning. 2 different monsters' move patterns were supported with the define (Focus On Player or Move Randomly). A Sleep(100) was used in loop to control the update speed.
The green node represents player, and red nodes represent monsters. I feel sorry for my art works.
No memory leak. Some warnings about pdb files in release version.
If there are any questions, please let me know.


V1.01
A input manager was added into Engine/Manager/InputManager.h  , a enum class(Key) was used to define the name of each key

You can close the application by press ESC

You can move by press A,S,D,W, it support 8 directions move
I finished the physics calculation about player's movement using Euler method. 
Since now there still a lot of problems to solve, I didn't make a physics Manager.

Besides, I made a singleton for TimeManager in Engine/Manager/TimeManager.h to offer information for other sub-system. It will only update in the main loop.
FPS will be printed in the output window.

Thank you for checking, if there are any questions, please let me know.

V1.02
Implement Big end and small end transfer for some types such as INT, FLOAT
Implement Vector4, Matrix, Triangle, Cube.
Use ForceInline, delete -inl.h file, reconstruct all classes.
Reconstruct all files in .h and .cpp. Uniform all files format and style(really time consuming, but really learned a lot)
Add support for Weak pointer and strong pointer   // in file Engine\Core\Pointers.h and Engine\Core\Private\Pointers.cpp
Add support for Container: LinkedList and List    // in file Engine\Core\Container.h 
Add unitTest for container and smart pointer      // in EngineTest\unit_test.cpp


V1.03
Reconstruct files structure of Engine project
Add simple api for unique pointer																				// in Engine/Core/Template/Pointer.h
Add RenerObject and RenderManager, RenderManager is a subsystem in this game which control all render objects.  // Engine/SubSystem/RenderManager.h
A renderObject is related to a gameobject																		// Engine/Subsystem/RenderObject.h
Replace normal pointer to smart pointer	as more as possible														// Game/main.cpp
Add StringPool, HashString and pooledString and their functions							// Engine/Core/String/


V1.04 
Reconstruct engine(maybe the last time), change file structure, change code standard.
Add stringPool into use																				         // Init in Engine/Engine/Private/Engine.cpp and use in Object/Actor.h (which store type and name)
add actor and pawn object structure																			 // Actor own gameObject and observe components, pawn inherites from Actor and observe a controller
add three kinds of controller, which control pawns, include player and monsters							     // Engine/Controller/Controller.h 
add ControllerManager, singleton which owns all controllers	(should move out of engine in the future)		 // Engine/Controller/ControllerManager.h
add WorldManager, singleton which owns all actors and pawns (Game Scene)									 // Engine/Manager/WorldManager.h	
fix bugs in FixedSizeAllocator

V1.05
add Physics objects, controllered by controller and update gameobject's parameters							 // Engine/Subsystem/PhysicsObject.h
add Physics Manager, singleton who owns all physics objects and update them				                     // Engine/SubSystem/PhysicsManage.h 
add Engine states, add Engine functions, rewrite code in Game/main.cpp relate to new structure				 // Game/main.cpp    Engine/Engine/Engine.h
add HeapManage, singleton init when the engine start. control all other new									 // Engine/Core/Memory/HeapManager.h
separate new into a new file, overload new use functions in HeapManager                                      // Engine/Core/Memory/New.h
Control all other singletons in Engine. Add static function Create, Get, Destroy into all singletons         // All singletons
add box2D class into gameobject