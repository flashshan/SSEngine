#include "Object\Actor.h"
#include "Manager\WorldManager.h"
#include "Manager\RealTimeManager.h"

Actor::~Actor()
{
}

void Actor::InitCalculation()
{
	gameObject_->SetObjectToWorld(Matrix::CreateTranslate(gameObject_->GetLocation()) * Matrix::CreateZRotation(gameObject_->GetRotation().Z) * Matrix::CreateScale(gameObject_->GetScale()));
	gameObject_->SetWorldToObject(gameObject_->GetObjectToWorld().GetInverse());
}

void Actor::PreCalculation()
{
	// currently support 2D matrix
	if (!isStatic_)
	{
		gameObject_->SetObjectToWorld(Matrix::CreateTranslate(gameObject_->GetLocation()) * Matrix::CreateZRotation(gameObject_->GetRotation().Z) * Matrix::CreateScale(gameObject_->GetScale()));
		gameObject_->SetWorldToObject(gameObject_->GetObjectToWorld().GetInverse());
	}
}

void Actor::ActualUpdate()
{
	handleCollision();

	if (!isStatic_)
	{
		gameObject_->Update();
	}
}

void Actor::handleCollision()
{
	if (!canCollide_ || isStatic_) return;

	WorldManager *temp = WorldManager::GetInstance();
	size_t count = temp->GetActorNumber();
	for (size_t i = 0; i < count;++i)
	{
		if (&(*temp->GetActor(i)) != this)
		{
			handleCollisionWith(*temp->GetActor(i));
		}
	}
	count = temp->GetPlayerNumber();
	for (size_t i = 0; i < count;++i)
	{
		if (&(*temp->GetPlayer(i)) != this)
		{
			handleCollisionWith(*temp->GetPlayer(i));
		}
	}
	count = temp->GetPawnNumber();
	for (size_t i = 0; i < count;++i)
	{
		if (&(*temp->GetPawn(i)) != this)
		{
			handleCollisionWith(*temp->GetPawn(i));
		}
	}
}

enum class CollideType {
	EPass,
	EEnter,
	ELeave,
	EStay,
	ENone
};

void Actor::handleCollisionWith(Actor& i_other)
{
	// precalculation, most actors were kick out by precalculation
	float tickTime = RealTimeManager::GetInstance()->GetLastFrameTimeS();
	if ((GetActorLocation() + Vector3(GetBoundingBox().GetCenter())).Distance(i_other.GetActorLocation() + Vector3(i_other.GetBoundingBox().GetCenter())) - (GetActorVelocity().Length() + i_other.GetActorVelocity().Length()) * tickTime >
		GetBoundingBox().GetExtend().Length() + i_other.GetBoundingBox().GetExtend().Length())
	{
		return;
	}
	
	// Magic code for calculation collision betwwen this and i_other, content from lecture 8
	Matrix thisToOther = i_other.gameObject_->GetWorldToObject() * gameObject_->GetObjectToWorld();
	Vector4 XExtendInOther = thisToOther.MultiplyLeft(Vector4(GetBoundingBox().GetExtend().X, 0.0f, 0.0f, 0.0f));
	Vector4 YExtendInOther = thisToOther.MultiplyLeft(Vector4(0.0f, GetBoundingBox().GetExtend().Y, 0.0f, 0.0f));

	float XProjectOnOther = Math::Abs(XExtendInOther.X) + Math::Abs(YExtendInOther.X);
	float YProjectOnOther = Math::Abs(XExtendInOther.Y) + Math::Abs(YExtendInOther.Y);

	float XExtend = i_other.GetBoundingBox().GetExtend().X + XProjectOnOther;
	float YExtend = i_other.GetBoundingBox().GetExtend().Y + YProjectOnOther;

	Vector3 centerOnOther = Vector3(thisToOther.MultiplyLeft(Vector4(GetBoundingBox().GetCenter())));
	Vector3 relativeVelocity = Vector3(thisToOther.MultiplyLeft(Vector4(GetActorVelocity(), 0.0f))) - i_other.GetActorVelocity();

	float timeCloseX = (i_other.GetBoundingBox().GetCenter().X - XExtend - centerOnOther.X) / relativeVelocity.X;
	float timeOpenX = (i_other.GetBoundingBox().GetCenter().X + XExtend - centerOnOther.X) / relativeVelocity.X;
	if (timeOpenX < timeCloseX) Basic::Swap(timeCloseX, timeOpenX);

	float timeCloseY = (i_other.GetBoundingBox().GetCenter().Y - YExtend - centerOnOther.Y) / relativeVelocity.Y;
	float timeOpenY = (i_other.GetBoundingBox().GetCenter().Y + YExtend - centerOnOther.Y) / relativeVelocity.Y;
	if (timeOpenY < timeCloseY) Basic::Swap(timeCloseY, timeOpenY);

	CollideType XType, YType;
	if (timeCloseX < 0.0f)
	{
		if (timeOpenX < 0.0f)
		{
			return;
		}
		else if(timeOpenX < tickTime)
		{
			XType = CollideType::ELeave;
		}
		else
		{
			XType = CollideType::EStay;
		}
	}
	else if (timeCloseX < tickTime)
	{
		if (timeOpenX < tickTime)
		{
			XType = CollideType::EPass;
		}
		else
		{
			XType = CollideType::EEnter;
		}
	}
	else
	{
		return;
	}

	if (timeCloseY < 0.0f)
	{
		if (timeOpenY < 0.0f)
		{
			return;
		}
		else if (timeOpenY < tickTime)
		{
			YType = CollideType::ELeave;
		}
		else
		{
			YType = CollideType::EStay;
		}
	}
	else if (timeCloseY < tickTime)
	{
		if (timeOpenY < tickTime)
		{
			YType = CollideType::EPass;
		}
		else
		{
			YType = CollideType::EEnter;
		}
	}
	else
	{
		return;
	}

	if ((XType == CollideType::EEnter && YType == CollideType::EEnter) || 
		(XType == CollideType::EEnter && YType == CollideType::EStay) || (XType == CollideType::EStay && YType == CollideType::EEnter))
	{
		DEBUG_PRINT("%s enter into %s", GetName(), i_other.GetName());
	}
	else if((XType == CollideType::ELeave && YType == CollideType::ELeave) || 
		(XType == CollideType::ELeave && YType == CollideType::EStay) || (XType == CollideType::EStay && YType == CollideType::ELeave))
	{
		DEBUG_PRINT("%s leave from %s", GetName(), i_other.GetName());
	}
	else if ((XType == CollideType::EEnter && (YType == CollideType::ELeave || YType == CollideType::EPass) && timeCloseX < timeOpenY) || ((XType == CollideType::ELeave || XType == CollideType::EPass) && YType == CollideType::EEnter && timeCloseY < timeOpenX) ||
		(XType == CollideType::ELeave && YType == CollideType::EPass && timeOpenX > timeCloseY) || (XType == CollideType::EPass && YType == CollideType::ELeave && timeOpenY > timeCloseX) ||
		(XType == CollideType::EStay && YType == CollideType::EPass) || (XType == CollideType::EPass && YType == CollideType::EStay) ||
		(XType == CollideType::EPass && YType == CollideType::EPass && timeOpenX > timeCloseY && timeOpenY > timeCloseX))
	{
		DEBUG_PRINT("%s collide with %s", GetName(), i_other.GetName());
	}
}