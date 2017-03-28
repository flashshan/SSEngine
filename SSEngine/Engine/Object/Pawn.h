#pragma once

#include "Actor.h"
#include "Controller\IController.h"

class Pawn : public Actor
{
public:
	FORCEINLINE Pawn();
	explicit FORCEINLINE Pawn(const Transform &i_transform, const char *i_name = "defaultName", const char *i_type = "defaultType");
	FORCEINLINE Pawn(const Pawn &i_other);
	
	FORCEINLINE Pawn(Pawn &&i_other);
	virtual ~Pawn();

	FORCEINLINE Pawn& operator =(const Pawn &i_other);
	FORCEINLINE Pawn& operator =(Pawn &&i_other);

	FORCEINLINE void SetController(const StrongPtr<IController> i_controller);

	virtual void EarlyUpdate() override {}
	virtual void Update() override;
	virtual void LateUpdate() override {}

private:
	// Controller's life cycle is longer than pawn
	WeakPtr<IController> controller_;
};











// implement forceinline 
// controller can not be copy or move

FORCEINLINE Pawn::Pawn()
	: Actor(), controller_()
{
}

FORCEINLINE Pawn::Pawn(const Transform &i_transform, const char *i_name, const char *i_type)
	: Actor(i_transform, i_name, i_type), controller_()
{
}

FORCEINLINE Pawn::Pawn(const Pawn &i_other)
	: Actor(i_other), controller_()
{
}

FORCEINLINE Pawn::Pawn(Pawn &&i_other)
	: Actor(std::move(i_other)), controller_()
{
}



FORCEINLINE Pawn& Pawn::operator =(const Pawn &i_other)
{
	Actor::operator=(i_other);
}

FORCEINLINE Pawn& Pawn::operator =(Pawn &&i_other)
{
	Actor::operator=(std::move(i_other));
}

FORCEINLINE void Pawn::SetController(StrongPtr<IController> i_controller)
{
	controller_ = i_controller;
}








// implement forceinline
