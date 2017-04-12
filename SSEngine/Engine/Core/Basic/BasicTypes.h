#pragma once

#include "Target.h"

// math define
#define PI 3.1415926535897932f
#define HALF_PI 1.57079632679f
#define INV_PI 0.31830988618f

#define EULER_NUMBER 2.71828182845904523536f

#define RAND_MAX 0x7fff
#define SMALL_NUMBER 1.e-8f
#define BIG_NUMBER 3.4e+38f

#define MAX_NAME_LENGTH 50

#define SIGN_BIT ((1 << 31))

#ifdef TARGET_WINDOWS

typedef char					int8;
typedef unsigned char			uint8;
typedef short					int16;
typedef unsigned short			uint16;
typedef int						int32;
typedef unsigned int			uint32;
typedef long long				int64;
typedef unsigned long long		uint64;

union U32F32
{
	uint32 asUint32;
	float asFloat32;
};
union U64F64
{
	uint64 asUint64;
	double asFloat64;
};

#ifdef _WIN64
typedef long long				intPtr;
typedef unsigned long long      uintPtr;

#else
typedef int						intPtr;
typedef unsigned int			uintPtr;

#endif // _WIN64

#endif // WINDOW