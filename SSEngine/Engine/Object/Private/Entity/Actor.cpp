#include "Object\Entity\Actor.h"
#include "Manager\WorldManager.h"

Actor::~Actor()
{
}

void Actor::InitCalculation()
{
	gameObject_->CalculateMatrix();
}

void Actor::PreCalculation()
{
	// currently support 2D matrix
	if (gameObject_->GetMobility() != Mobility::EStatic)
	{
		gameObject_->CalculateMatrix();
	}
}

void Actor::ActualUpdate()
{
	if (gameObject_->GetMobility() != Mobility::EStatic)
	{
		if (collisionObject_->GetIsCollide())
		{
			gameObject_->Update(collisionObject_->GetCollideRecord());
		}
		else
		{
			gameObject_->Update();
		}
	}
}

