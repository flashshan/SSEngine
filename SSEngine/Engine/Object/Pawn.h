#pragma once

#include "Actor.h"
#include "Controller\IController.h"

class Pawn : public Actor
{
public:
	inline Pawn();
	explicit inline Pawn(const Transform &i_transform, const char *i_name = "defaultName", const char *i_type = "defaultType", bool i_static = false);
	inline Pawn(const Pawn &i_other);
	inline Pawn(Pawn &&i_other);
	virtual ~Pawn();

	inline Pawn& operator =(const Pawn &i_other);
	inline Pawn& operator =(Pawn &&i_other);

	FORCEINLINE void SetController(const WeakPtr<IController> &i_controller);

	virtual void EarlyUpdate() override;
	virtual void Update() override;
	virtual void LateUpdate() override;

private:
	// Controller's life cycle is longer than pawn
	WeakPtr<IController> controller_;
};











// implement forceinline 
// controller can not be copy or move

inline Pawn::Pawn()
	: Actor(), controller_()
{
}

inline Pawn::Pawn(const Transform &i_transform, const char *i_name, const char *i_type, const bool i_static)
	: Actor(i_transform, i_name, i_type, i_static), controller_()
{
}

inline Pawn::Pawn(const Pawn &i_other)
	: Actor(i_other), controller_()
{
}

inline Pawn::Pawn(Pawn &&i_other)
	: Actor(std::move(i_other)), controller_()
{
}

inline Pawn& Pawn::operator =(const Pawn &i_other)
{
	Actor::operator=(i_other);
}

inline Pawn& Pawn::operator =(Pawn &&i_other)
{
	Actor::operator=(std::move(i_other));
}

FORCEINLINE void Pawn::SetController(const WeakPtr<IController> &i_controller)
{
	controller_ = i_controller;
}








// implement forceinline
