#pragma once

#include "Core\Template\Pointers.h"

class Pawn;
struct Vector3;

class IController
{
public:
	virtual void SetPawn(const StrongPtr<Pawn> &i_pawn) = 0;
	virtual bool IsValid() const = 0;
	virtual void UpdateController() = 0;
	virtual ~IController() {};
};
