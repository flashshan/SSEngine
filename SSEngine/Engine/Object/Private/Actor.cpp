#include "Object\Actor.h"
#include "Manager\WorldManager.h"

Actor::~Actor()
{
}

void Actor::InitCalculation()
{
	gameObject_->SetObjectToWorld(Matrix::CreateScale(gameObject_->GetScale()) * Matrix::CreateZRotation(Math::DegreesToRadians(gameObject_->GetRotation().Roll)) * Matrix::CreateTranslate(gameObject_->GetLocation()));
	gameObject_->SetWorldToObject(gameObject_->GetObjectToWorld().GetInverse());
}

void Actor::PreCalculation()
{
	// currently support 2D matrix
	if (!gameObject_->GetStatic())
	{
		gameObject_->SetObjectToWorld(Matrix::CreateScale(gameObject_->GetScale()) * Matrix::CreateZRotation(Math::DegreesToRadians(gameObject_->GetRotation().Roll)) * Matrix::CreateTranslate(gameObject_->GetLocation()));
		gameObject_->SetWorldToObject(gameObject_->GetObjectToWorld().GetInverse());
	}
}

void Actor::ActualUpdate()
{
	if (!gameObject_->GetStatic())
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

