#pragma once

#include "Core\CoreMinimal.h"
#include "Core\Template\Pointers.h"

class Pawn;
struct Vector3;

class IController
{
public:
	virtual void SetPawn(StrongPtr<Pawn> &i_pawn) = 0;
	virtual Pawn *GetPawn() const = 0;

	virtual ~IController() {}
};
