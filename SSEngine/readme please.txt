Introduction in Chinese:
文件结构及主要功能： 
Core\Basic\ 
Assert: 断言检测， SLOW_ASSERT包含console print位置，错误信息等
Target & BasicType: 平台无关层数据类型定义
BasicFunction: 基本方法，大小端数据转化，float检测及比较，rand方法等
FileIO & LuaData: : 文件载入及Lua支持，Lua数组读取
Core\Math\
包含Box2D, Vector2,3,4, Matrix等类，Matrix运算使用SIMD
Core\Memory\
内存管理，包括堆分配器，固定大小内存池分配器，多种new重载，内存管理单例等等
Core\String\
包含字符串哈希类，字符串池及池内字符串类（字符串池中 字符串的封装类）。
Core\Template
包含智能指针，strong,weak,unique pointer的实现。包含各种容器。包括Array, LinkedList, List, Queue,Stack, CircleQueue等. 部分容器为stl容器的包装
Core\Thread
包含互斥量，信号量，事件，单写多读锁的封装

Object\
包含GameObject, Actor和Pawn，GameObject保存游戏对象实际数据，Actor拥有GaneObject和引擎子系统组件及其他组件，并为gameObject的行为提供接口，pawn继承自Actor，监视一个controller并被其控制
Controller\
包含一个抽象类IController，一个玩家控制器，两种怪物控制器（以后会将自定义控制器移除引擎），以及一个控制器管理器单例，控制器监视一个pawn并控制其行为，控制器管理器拥有所有的控制器并负责更新。
SubSystem\Job
作业系统，保持细粒度的多线程任务。JobSystem单例管理一个名字和人任务队列及线程队列对应的map，可以把任务推送到任务队列，对应的线程队列持续从任务队列中提取任务并执行。
Jobs\
作业对象类，包含一个抽象类IJob，目前包含JobLoadActor和JobLoadPawn作业，均为较费时的从lua脚本读取数据生成对象的操作
SubSystem\Render
引擎渲染子系统，包括渲染对象（被Actor监视）和渲染管理器（拥有并负责管理和更新所有actor和pawn的renderObject）。同时渲染管理器管理所有载入游戏的图像资源，保证资源的内存唯一性。
SubSystem\Physics
引擎物理子系统，包括物理对象（被Actor监视）和物理管理器（拥有并管理所有物理对象），物理对象从Actor的接口由Controller提供数据，Euler Method模拟速度变化和运动
Manager\
包含游戏时间和现实时间控制器（由CPU高精度时钟实现），并提供时间差给主线程锁定帧率
输入控制器（用GLib回调方法更新）
世界控制器，世界控制器拥有所有Actor和Pawn对象，提供Actor和Pawn的生成方法，生成Pawn对象时会根据对象类型链接Controller。世界控制器保证对象在帧前被加入或删除并保证帧内更新行为的安全性
Component\
包含基本的Componenet transform及抽象类IComponent
Engine\
包含引擎对象，引擎的入口和出口。

EngineTest
main.cpp包含各个系统的UnitTest

Game
Game\Assets
包含游戏中用到的lua脚本和一些自制的简单sprite

main.cpp
高层游戏行为，目前相当简单，启动引擎和各个子系统，生成Actor，Pawn并更新。最终终止引擎，并检测内存泄漏




Update list:
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
Add simple api for unique pointer																				// in Engine\Core\Template\Pointer.h
Add RenerObject and RenderManager, RenderManager is a subsystem in this game which control all render objects.  // Engine\SubSystem\RenderManager.h
A renderObject is related to a gameobject																		// Engine\Subsystem\RenderObject.h
Replace normal pointer to smart pointer	as more as possible														// Game\main.cpp
Add StringPool, HashString and pooledString and their functions							// Engine/Core/String/


V1.04 
Reconstruct engine(maybe the last time), change file structure, change code standard.
Add stringPool into use																				         // Init in Engine/Engine/Private/Engine.cpp and use in Object/Actor.h (which store type and name)
add actor and pawn object structure																			 // Actor own gameObject and observe components, pawn inherites from Actor and observe a controller
add three kinds of controller, which control pawns, include player and monsters							     // Engine\Controller\Controller.h 
add ControllerManager, singleton which owns all controllers	(should move out of engine in the future)		 // Engine\Controller\ControllerManager.h
add WorldManager, singleton which owns all actors and pawns (Game Scene)									 // Engine\Manager\WorldManager.h	
fix bugs in FixedSizeAllocator


V1.05
add Physics objects, controllered by controller and update gameobject's parameters							 // Engine\Subsystem\PhysicsObject.h
add Physics Manager, singleton who owns all physics objects and update them				                     // Engine\SubSystem\PhysicsManage.h 
add Engine states, add Engine functions, rewrite code in Game/main.cpp relate to new structure				 // Game\main.cpp    Engine\Engine\Engine.h
add HeapManage, singleton init when the engine start. control all other new									 // Engine\Core/Memory\HeapManager.h
separate new into a new file, overload new use functions in HeapManager                                      // Engine\Core/Memory\New.h
Control all other singletons in Engine. Add static function Create, Get, Destroy into all singletons         // All singletons
add box2D class into gameobject


V1.06
add drag into physicsObject, the drag = -velocity ^ 2 * k;													 // Engine\Subsystem\PhysicsObject.h
add support for CircleQueue																					 // Engine\Core\Template\CircleQueue.h
separate loadfile function into core																		 // Engine\Core\Basic\FileIO.h												
add lua into ThirdParty\Lua, add support for lua															 // Engine\Core\Basic\LuaData.h
add Assets folder in Game and add lua script																 // Game\Assets\Data\Player1.lua
add lua load in worldManager to load actor and pawn from lua, used in Game\main.cpp        					 // Engine\Manager\WorldManager.h
modify EngineTest project into a window application, add UnitTest for lua and circleQueue					 // EngineTest\main.cpp

V1.07
big changes, provide multi-thread features
add job system into the Engine (testing), initial in Engine and ShutDown when engine shut down				 // Engine\SubSystem\Job
add some simple jobs (Load Actor and Pawn from lua) and use these jobs in Game\main.cpp						 // Engine\Jobs\
change heapAllocator and fixedSizeAllocator's operations to thread-safe operations 
// assignment notification:
Support all necessary functions within Vector4 and Matrix, some of them are done in SIMD					 // Engine\Core\Math\

V1.08
change container for PhysicsManager and RenderManger
support priority in RenderManager, objects with higher priority are rendered later							 // Engine\SubSystem\Render\RenderManager.h
add collision into actor, support debug information when they collide(in test)								 // Engine\Object\Private\Actor.cpp
add properties (canCollide, isStatic, isActive) into actor, which control collision and update				 // Engine\Object\Actor.h
change the flows of actors were added into the world. All new actors are added into a queue which will not be update in this frame,
in next frame, actors in the queue will be pushed into array which allow actors to update					 // Engine\Manager\WorldManager.h
change necessary functions in all singletons to be thread-safe to some extent, mainly controlled by critical section.
Fixed bugs within RealTimeManager, add Sleep in main thread to lock frame rate to 60 fps.					 // Engine\Engine\Engine.h
collision bugs fixed, DEBUG_PRINT when they collide