#pragma once

#include "Core\Template\Pointers.h"
class Pawn;

// Interface class
class IController
{
public:
	virtual void SetPawn(const WeakPtr<Pawn>& i_pawn) = 0;
	virtual WeakPtr<Pawn> GetControlledPawn() const = 0;
	virtual bool IsValid() const = 0;
	virtual void UpdateController() = 0;
	virtual ~IController() {};
};
