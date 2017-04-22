#include "Object\Actor.h"
#include "Manager\WorldManager.h"

Actor::~Actor()
{
}

void Actor::InitCalculation()
{
	gameObject_->SetObjectToWorld(Matrix::CreateTranslate(gameObject_->GetLocation()) * Matrix::CreateZRotation(Math::DegreesToRadians(gameObject_->GetRotation().Roll)) * Matrix::CreateScale(gameObject_->GetScale()));
	gameObject_->SetWorldToObject(gameObject_->GetObjectToWorld().GetInverse());
}

void Actor::PreCalculation()
{
	// currently support 2D matrix
	if (!gameObject_->GetStatic())
	{
		gameObject_->SetObjectToWorld(Matrix::CreateTranslate(gameObject_->GetLocation()) * Matrix::CreateZRotation(Math::DegreesToRadians(gameObject_->GetRotation().Roll)) * Matrix::CreateScale(gameObject_->GetScale()));
		gameObject_->SetWorldToObject(gameObject_->GetObjectToWorld().GetInverse());
	}
}

void Actor::ActualUpdate()
{
	if (!gameObject_->GetStatic())
	{
		gameObject_->Update();
	}
}

