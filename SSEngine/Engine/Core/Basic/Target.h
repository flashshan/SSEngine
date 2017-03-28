#pragma once

// Target Platform

#define TARGET_WINDOWS

//#define TARGET_PS4
//#define TARGET_XB1

//#define BUILD_DEBUG
//#define BUILD_DEVELOPMENT
//#define BUILD_RELEASE


#ifdef TARGET_WINDOWS

#define ALIGN(n) __declspec(align(n))

#define FORCEINLINE __forceinline

#define DLLEXPORT __declspec(dllexport)
#define DLLIMPORT __declspec(dllimport)

#ifdef _WIN64
#define TARGET_PLATFORM "window 64"

#else
#define TARGET_PLATFORM "window 32"

#endif // _WIN64

#endif // WINDOW


