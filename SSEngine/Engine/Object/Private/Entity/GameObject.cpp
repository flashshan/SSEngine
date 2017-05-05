#include "Object\Entity\GameObject.h"
#include "Manager\RealTimeManager.h"

#include "SubSystem\Collision\CollisionObject.h"

void GameObject::CalculateMatrix()
{
	objectToWorld_ = Matrix::CreateScale(transform_.GetScale()) * Matrix::CreateZRotation(Math::DegreesToRadians(transform_.GetRotation().Roll)) * Matrix::CreateTranslate(transform_.GetLocation());
	worldToObject_ = objectToWorld_.GetInverse();
}


void GameObject::Update()
{
	transform_.Translate(velocity_ * RealTimeManager::GetInstance()->GetLastFrameTimeS());
}

void GameObject::Update(Array<CollideRecord> *i_record)
{
	size_t count = i_record->Size();
	for (size_t i = 0; i < count; ++i)
	{
		transform_.Translate(i_record->operator[](i).Velocity * i_record->operator[](i).Time);
	}
	SetVelocity(i_record->Back().Velocity);
}