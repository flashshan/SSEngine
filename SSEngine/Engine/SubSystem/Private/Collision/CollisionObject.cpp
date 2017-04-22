#include "SubSystem\Collision\CollisionObject.h"

#include "Manager\RealTimeManager.h"
#include "Object\Actor.h"

enum class CollideResult : uint8 {
	ENone = 0,
	EPass = 1,
	EEnter = 2,
	ELeave = 3,
	EStay = 4
};

CollideResult collisionOneWayCheck(const CollisionObject &i_object1, const CollisionObject &i_object2, const Matrix &i_matrix1To2, float i_tickTime)
{
	WeakPtr<GameObject> gameObject1 = i_object1.GetGameObject(), gameObject2 = i_object2.GetGameObject();
	Vector4 XExtend1In2 = i_matrix1To2.MultiplyLeft(Vector4(gameObject1->GetBoundingBox().GetExtend().X, 0.0f, 0.0f, 0.0f));
	Vector4 YExtend1In2 = i_matrix1To2.MultiplyLeft(Vector4(0.0f, gameObject1->GetBoundingBox().GetExtend().Y, 0.0f, 0.0f));

	float XProject1In2 = Math::AbsF(XExtend1In2.X) + Math::AbsF(YExtend1In2.X);
	float YProject1In2 = Math::AbsF(XExtend1In2.Y) + Math::AbsF(YExtend1In2.Y);

	float XExtend = gameObject2->GetBoundingBox().GetExtend().X + XProject1In2;
	float YExtend = gameObject2->GetBoundingBox().GetExtend().Y + YProject1In2;

	Vector3 center1In2 = Vector3(i_matrix1To2.MultiplyLeft(Vector4(gameObject1->GetBoundingBox().GetCenter())));
	Vector3 relativeVelocity = Vector3(i_matrix1To2.MultiplyLeft(Vector4(gameObject1->GetVelocity(), 0.0f))) - gameObject2->GetVelocity();

	float timeCloseX = (gameObject2->GetBoundingBox().GetCenter().X - XExtend - center1In2.X) / relativeVelocity.X;
	float timeOpenX = (gameObject2->GetBoundingBox().GetCenter().X + XExtend - center1In2.X) / relativeVelocity.X;
	if (timeOpenX < timeCloseX) Basic::Swap(timeCloseX, timeOpenX);

	float timeCloseY = (gameObject2->GetBoundingBox().GetCenter().Y - YExtend - center1In2.Y) / relativeVelocity.Y;
	float timeOpenY = (gameObject2->GetBoundingBox().GetCenter().Y + YExtend - center1In2.Y) / relativeVelocity.Y;
	if (timeOpenY < timeCloseY) Basic::Swap(timeCloseY, timeOpenY);

	CollideResult XType, YType;
	if (timeCloseX < 0.0f)
	{
		if (timeOpenX < 0.0f)
		{
			return CollideResult::ENone;
		}
		else if (timeOpenX < i_tickTime)
		{
			XType = CollideResult::ELeave;
		}
		else
		{
			XType = CollideResult::EStay;
		}
	}
	else if (timeCloseX < i_tickTime)
	{
		if (timeOpenX < i_tickTime)
		{
			XType = CollideResult::EPass;
		}
		else
		{
			XType = CollideResult::EEnter;
		}
	}
	else
	{
		return CollideResult::ENone;
	}

	if (timeCloseY < 0.0f)
	{
		if (timeOpenY < 0.0f)
		{
			return CollideResult::ENone;
		}
		else if (timeOpenY < i_tickTime)
		{
			YType = CollideResult::ELeave;
		}
		else
		{
			YType = CollideResult::EStay;
		}
	}
	else if (timeCloseY < i_tickTime)
	{
		if (timeOpenY < i_tickTime)
		{
			YType = CollideResult::EPass;
		}
		else
		{
			YType = CollideResult::EEnter;
		}
	}
	else
	{
		return CollideResult::ENone;
	}

	// Return collide type based on XType and YType
	if (XType == CollideResult::EStay && YType == CollideResult::EStay)
	{
		return CollideResult::EStay;
	}
	else if ((XType == CollideResult::EEnter && YType == CollideResult::EEnter) ||
		(XType == CollideResult::EEnter && YType == CollideResult::EStay) || (XType == CollideResult::EStay && YType == CollideResult::EEnter))
	{
		return CollideResult::EEnter;
	}
	else if ((XType == CollideResult::ELeave && YType == CollideResult::ELeave) ||
		(XType == CollideResult::ELeave && YType == CollideResult::EStay) || (XType == CollideResult::EStay && YType == CollideResult::ELeave))
	{
		return CollideResult::ELeave;
	}
	else if ((XType == CollideResult::EEnter && (YType == CollideResult::ELeave || YType == CollideResult::EPass) && timeCloseX < timeOpenY) || ((XType == CollideResult::ELeave || XType == CollideResult::EPass) && YType == CollideResult::EEnter && timeCloseY < timeOpenX) ||
		(XType == CollideResult::ELeave && YType == CollideResult::EPass && timeOpenX > timeCloseY) || (XType == CollideResult::EPass && YType == CollideResult::ELeave && timeOpenY > timeCloseX) ||
		(XType == CollideResult::EStay && YType == CollideResult::EPass) || (XType == CollideResult::EPass && YType == CollideResult::EStay) ||
		(XType == CollideResult::EPass && YType == CollideResult::EPass && timeOpenX > timeCloseY && timeOpenY > timeCloseX))
	{
		return CollideResult::EPass;
	}
	return CollideResult::ENone;
}

void CollisionObject::handleCollisionWith(const CollisionObject& i_other)
{
	if (!i_other.active_) return;

	// precalculation, most actors were kick out by precalculation
	float tickTime = RealTimeManager::GetInstance()->GetLastFrameTimeS();
	if ((gameObject_->GetLocation() + Vector3(gameObject_->GetBoundingBox().GetCenter())).Distance(i_other.gameObject_->GetLocation() + Vector3(i_other.gameObject_->GetBoundingBox().GetCenter())) - (gameObject_->GetVelocity().Length() + i_other.gameObject_->GetVelocity().Length()) * tickTime >
		gameObject_->GetBoundingBox().GetExtend().Length() + i_other.gameObject_->GetBoundingBox().GetExtend().Length())
	{
		return;
	}

	//double check collision betwwen this and i_other, content from lecture 8
	Matrix thisToOther = i_other.gameObject_->GetWorldToObject() * gameObject_->GetObjectToWorld();
	CollideResult thisWithOther = collisionOneWayCheck(*this, i_other, thisToOther, tickTime);
	if (thisWithOther == CollideResult::ENone) return;

	Matrix otherToThis = thisToOther.GetInverse();
	CollideResult otherWithThis = collisionOneWayCheck(i_other, *this, otherToThis, tickTime);

	Matrix test = thisToOther * otherToThis;
	if (otherWithThis == CollideResult::ENone) return;

	ASSERT(!(thisWithOther != otherWithThis && (otherWithThis == CollideResult::EEnter || otherWithThis == CollideResult::ELeave) &&
		(thisWithOther == CollideResult::EEnter || thisWithOther == CollideResult::ELeave)));

	CollideResult commonRes;
	if (thisWithOther == CollideResult::EPass || otherWithThis == CollideResult::EPass) commonRes = CollideResult::EPass;
	else if (thisWithOther != CollideResult::EStay) commonRes = thisWithOther;
	else commonRes = otherWithThis;

	//CollideResult commonRes = thisWithOther;

	ASSERT(gameObject_->GetOwner() != nullptr && i_other.gameObject_->GetOwner() != nullptr)
	if (commonRes == CollideResult::EEnter)
	{
		DEBUG_PRINT("%s enter into %s", gameObject_->GetOwner()->GetName(), i_other.gameObject_->GetOwner()->GetName());
	}
	else if (commonRes == CollideResult::ELeave)
	{
		DEBUG_PRINT("%s leave away %s", gameObject_->GetOwner()->GetName(), i_other.gameObject_->GetOwner()->GetName());
	}
	else if (commonRes == CollideResult::EPass)
	{
		DEBUG_PRINT("%s pass through %s", gameObject_->GetOwner()->GetName(), i_other.gameObject_->GetOwner()->GetName());
	}

}




void CollisionObject::DoCollision(size_t i_index, Array<CollisionElement> &i_collisionElements)
{
	size_t counter = i_collisionElements.Size();
	if (!active_) return;

	// check forward, don't look back
	for (size_t i = i_index + 1; i < counter; ++i)
	{
		if (i_collisionElements[i].Pointer->IsValid())
		{
			handleCollisionWith(*i_collisionElements[i].Pointer);
		}
	}
}

