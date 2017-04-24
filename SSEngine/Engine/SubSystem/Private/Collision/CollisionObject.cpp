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

//struct Collideinfo {
//	CollideResult Result;
//	float collideTime;
//
//};

CollideResult collisionOneWayCheck(CollisionObject &i_object1, CollisionObject &i_object2, Matrix &i_matrix1To2, const Vector3 &i_relativeVel1In2, float i_tickTime, float &o_closeTime)
{
	WeakPtr<GameObject> gameObject1 = i_object1.GetGameObject(), gameObject2 = i_object2.GetGameObject();
	Vector4 XExtend1In2 = Vector4(gameObject1->GetBoundingBox().GetExtend().X, 0.0f, 0.0f, 0.0f).Mul(i_matrix1To2);
	Vector4 YExtend1In2 = Vector4(0.0f, gameObject1->GetBoundingBox().GetExtend().Y, 0.0f, 0.0f).Mul(i_matrix1To2);

	float XProject1In2 = Math::AbsF(XExtend1In2.X) + Math::AbsF(YExtend1In2.X);
	float YProject1In2 = Math::AbsF(XExtend1In2.Y) + Math::AbsF(YExtend1In2.Y);

	float XExtend = gameObject2->GetBoundingBox().GetExtend().X + XProject1In2;
	float YExtend = gameObject2->GetBoundingBox().GetExtend().Y + YProject1In2;

	Vector3 center1In2 = Vector3(Vector4(gameObject1->GetBoundingBox().GetCenter()).Mul(i_matrix1To2));

	float timeCloseX = (gameObject2->GetBoundingBox().GetCenter().X - center1In2.X - XExtend) / i_relativeVel1In2.X;
	float timeOpenX = (gameObject2->GetBoundingBox().GetCenter().X - center1In2.X + XExtend) / i_relativeVel1In2.X;
	if (timeOpenX < timeCloseX) Basic::Swap(timeCloseX, timeOpenX);

	float timeCloseY = (gameObject2->GetBoundingBox().GetCenter().Y - center1In2.Y - YExtend) / i_relativeVel1In2.Y;
	float timeOpenY = (gameObject2->GetBoundingBox().GetCenter().Y - center1In2.Y + YExtend) / i_relativeVel1In2.Y;
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
		if (timeCloseX < 0.f || (timeCloseX > 0.f && timeCloseY > timeCloseX))
		{
			i_object1.SetIsCollide(true);
			i_object2.SetIsCollide(true);
			o_closeTime = timeCloseY;
			Vector3 newVelocity1(gameObject1->GetVelocity().X, gameObject2->GetVelocity().Y, 0.f), newVelocity2(gameObject2->GetVelocity().X, gameObject1->GetVelocity().Y, 0.f);
			i_object1.AddCollideRecord(CollideRecord(gameObject1->GetVelocity(), o_closeTime));
			i_object1.AddCollideRecord(CollideRecord(newVelocity1, i_tickTime - o_closeTime));
			i_object2.AddCollideRecord(CollideRecord(gameObject2->GetVelocity(), o_closeTime));
			i_object2.AddCollideRecord(CollideRecord(newVelocity2, i_tickTime - o_closeTime));
		}

		else if (timeCloseY < 0.f || (timeCloseY > 0.f && timeCloseX > timeCloseY))
		{
			i_object1.SetIsCollide(true);
			i_object2.SetIsCollide(true);
			o_closeTime = timeCloseX;
			Vector3 newVelocity1(gameObject2->GetVelocity().X, gameObject1->GetVelocity().Y, 0.f), newVelocity2(gameObject1->GetVelocity().X, gameObject2->GetVelocity().Y, 0.f);
			i_object1.AddCollideRecord(CollideRecord(gameObject1->GetVelocity(), o_closeTime));
			i_object1.AddCollideRecord(CollideRecord(newVelocity1, i_tickTime - o_closeTime));
			i_object2.AddCollideRecord(CollideRecord(gameObject2->GetVelocity(), o_closeTime));
			i_object2.AddCollideRecord(CollideRecord(newVelocity2, i_tickTime - o_closeTime));
		}
	
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
		if (timeCloseX < 0.f || (timeCloseX > 0.f && timeCloseY > timeCloseX))
		{
			i_object1.SetIsCollide(true);
			i_object2.SetIsCollide(true);
			o_closeTime = timeCloseY;
			Vector3 newVelocity1(gameObject1->GetVelocity().X, gameObject2->GetVelocity().Y, 0.f), newVelocity2(gameObject2->GetVelocity().X, gameObject1->GetVelocity().Y, 0.f);
			i_object1.AddCollideRecord(CollideRecord(gameObject1->GetVelocity(), o_closeTime));
			i_object1.AddCollideRecord(CollideRecord(newVelocity1, i_tickTime - o_closeTime));
			i_object2.AddCollideRecord(CollideRecord(gameObject2->GetVelocity(), o_closeTime));
			i_object2.AddCollideRecord(CollideRecord(newVelocity2, i_tickTime - o_closeTime));
		}
		else if (timeCloseY < 0.f || (timeCloseY > 0.f && timeCloseX > timeCloseY))
		{
			i_object1.SetIsCollide(true);
			i_object2.SetIsCollide(true);
			o_closeTime = timeCloseX;
			Vector3 newVelocity1(gameObject2->GetVelocity().X, gameObject1->GetVelocity().Y, 0.f), newVelocity2(gameObject1->GetVelocity().X, gameObject2->GetVelocity().Y, 0.f);
			i_object1.AddCollideRecord(CollideRecord(gameObject1->GetVelocity(), o_closeTime));
			i_object1.AddCollideRecord(CollideRecord(newVelocity1, i_tickTime - o_closeTime));
			i_object2.AddCollideRecord(CollideRecord(gameObject2->GetVelocity(), o_closeTime));
			i_object2.AddCollideRecord(CollideRecord(newVelocity2, i_tickTime - o_closeTime));
		}
		return CollideResult::EPass;
	}
	return CollideResult::ENone;
}

void CollisionObject::handleCollisionWith(CollisionObject& i_other)
{
	if (!i_other.active_) return;

	// precalculation, most actors were kick out by precalculation
	float tickTime = RealTimeManager::GetInstance()->GetLastFrameTimeS();
	if ((gameObject_->GetLocation() + Vector3(gameObject_->GetBoundingBox().GetCenter())).Distance(i_other.gameObject_->GetLocation() + Vector3(i_other.gameObject_->GetBoundingBox().GetCenter())) >
		(gameObject_->GetVelocity() - i_other.gameObject_->GetVelocity()).Length() * tickTime + gameObject_->GetBoundingBox().GetExtend().Length() + i_other.gameObject_->GetBoundingBox().GetExtend().Length())
	{
		return;
	}

	float outTime1;
	//double check collision betwwen this and i_other, content from lecture 8
	Matrix thisToOther = gameObject_->GetObjectToWorld() * i_other.gameObject_->GetWorldToObject();
	Vector3 relativeVelThisInOther = Vector3(Vector4(gameObject_->GetVelocity() - i_other.gameObject_->GetVelocity(), 0.f).Mul(i_other.gameObject_->GetWorldToObject()));
	CollideResult thisWithOther = collisionOneWayCheck(*this, i_other, thisToOther, relativeVelThisInOther, tickTime, outTime1);
	if (thisWithOther == CollideResult::ENone) return;

	/*Matrix otherToThis = i_other.gameObject_->GetObjectToWorld() * gameObject_->GetWorldToObject();
	Vector3 relativeVelOtherInThis = Vector3(Vector4(i_other.gameObject_->GetVelocity() - gameObject_->GetVelocity(), 0.f).Mul(gameObject_->GetWorldToObject()));
	CollideResult otherWithThis = collisionOneWayCheck(i_other, *this, otherToThis, relativeVelOtherInThis, tickTime, outTime2);

	if (otherWithThis == CollideResult::ENone) return;*/

	//ASSERT(!((thisWithOther == CollideResult::EEnter && otherWithThis == CollideResult::ELeave) || (thisWithOther == CollideResult::ELeave && otherWithThis == CollideResult::EEnter)));

	CollideResult commonRes;
	/*if ((thisWithOther == CollideResult::EEnter && otherWithThis == CollideResult::ELeave) || (thisWithOther == CollideResult::ELeave && otherWithThis == CollideResult::EEnter)) commonRes = CollideResult::ENone;
	else if (thisWithOther == CollideResult::EPass || otherWithThis == CollideResult::EPass) commonRes = CollideResult::EPass;
	else if (thisWithOther != CollideResult::EStay) commonRes = thisWithOther;
	else commonRes = otherWithThis;*/

	commonRes = thisWithOther;

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

	resetCollideinfo();
	// check forward, don't look back
	for (size_t i = i_index + 1; i < counter; ++i)
	{
		if (i_collisionElements[i].Pointer->IsValid())
		{
			handleCollisionWith(*i_collisionElements[i].Pointer);
		}
	}
}

