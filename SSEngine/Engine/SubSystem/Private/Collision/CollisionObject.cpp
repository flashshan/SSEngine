#include "SubSystem\Collision\CollisionObject.h"

#include "Manager\RealTimeManager.h"
#include "Object\Entity\Pawn.h"

enum class ECollideResult : uint8 {
	ENone = 0,
	EPass = 1,
	EEnter = 2,
	ELeave = 3,
	EStay = 4
};


void SetCollisionRecord(CollisionObject &i_object1, CollisionObject &i_object2, EAxis i_collideAxis, float i_timeClose, float i_tickTime)
{
	WeakPtr<GameObject> gameObject1 = i_object1.GetGameObject(), gameObject2 = i_object2.GetGameObject();

	i_object1.SetIsCollide(true);
	i_object2.SetIsCollide(true);

	WeakPtr<PhysicsObject> physicsObject1 = gameObject1->GetOwner()->GetPhysicsObject();
	WeakPtr<PhysicsObject> physicsObject2 = gameObject2->GetOwner()->GetPhysicsObject();
	float mass1 = 1.f, mass2 = 1.f;
	if (physicsObject1 && physicsObject2)
	{
		mass1 = physicsObject1->GetMass();
		mass2 = physicsObject2->GetMass();
	}
	else if (physicsObject1 && !physicsObject2)
	{
		mass1 = physicsObject1->GetMass();
		mass2 = mass1;
	}
	else if (!physicsObject1 && physicsObject2)
	{
		mass2 = physicsObject2->GetMass();
		mass1 = mass2;
	}
	
	Vector3 newVelocity1, newVelocity2;
	switch(i_collideAxis)
	{
	case EAxis::X:
		newVelocity1 = Vector3(((mass1 - mass2) * gameObject1->GetVelocity().X + 2 * mass2 * gameObject2->GetVelocity().X) / (mass1 + mass2), 0.f, gameObject1->GetVelocity().Y);
		newVelocity2 = Vector3(((mass2 - mass1) * gameObject2->GetVelocity().X + 2 * mass1 * gameObject1->GetVelocity().X) / (mass1 + mass2), 0.f, gameObject2->GetVelocity().Y);
		break;
	case EAxis::Y:
		newVelocity1 = Vector3(gameObject1->GetVelocity().X, ((mass1 - mass2) * gameObject1->GetVelocity().Y + 2 * mass2 * gameObject2->GetVelocity().Y) / (mass1 + mass2), 0.f);
		newVelocity2 = Vector3(gameObject2->GetVelocity().X, ((mass2 - mass1) * gameObject2->GetVelocity().Y + 2 * mass1 * gameObject1->GetVelocity().Y) / (mass1 + mass2), 0.f);
		break;
	case EAxis::Z:
	default:
		break;
	}

	i_object1.AddCollideRecord(CollideRecord(gameObject1->GetVelocity(), i_timeClose));
	i_object1.AddCollideRecord(CollideRecord(newVelocity1, i_tickTime - i_timeClose));
	i_object2.AddCollideRecord(CollideRecord(gameObject2->GetVelocity(), i_timeClose));
	i_object2.AddCollideRecord(CollideRecord(newVelocity2, i_tickTime - i_timeClose));
}

// will be implemented by messaging system in the future 
void HardCodeCollisionEvents(CollisionObject &i_object1, CollisionObject &i_object2)
{
	Actor *owner1 = i_object1.GetGameObject()->GetOwner();
	Actor *owner2 = i_object2.GetGameObject()->GetOwner();
	ASSERT(owner1 != nullptr);
	ASSERT(owner2 != nullptr);

	HashedString type1 = owner1->GetType(), type2 = owner2->GetType();
	HashedString player = HashedString("Player"), wall = HashedString("Wall"), focusMonster = HashedString("FocusMoveMonster"),  bullet = HashedString("Bullet");
	if (type1 == player && type2 == focusMonster)
	{
		Pawn *player = dynamic_cast<Pawn*>(owner1);
		player->TakeDamage();
		if (player->GetHP() == 0)
		{
			player->DestroyActor();
		}
		owner2->DestroyActor();
	}
	else if (type1 == focusMonster && type2 == player)
	{
		Pawn *player = dynamic_cast<Pawn*>(owner2);
		player->TakeDamage();
		if (player->GetHP() == 0)
		{
			player->DestroyActor();
		}
		owner1->DestroyActor();
	}
	else if (type1 == bullet && type2 == wall)
	{
		owner1->DestroyActor();
	}
	else if (type1 == wall && type2 == bullet)
	{
		owner2->DestroyActor();
	}
	else if (type1 == bullet && type2 == focusMonster)
	{
		Pawn *monster = dynamic_cast<Pawn*>(owner2);
		monster->TakeDamage();
		if (monster->GetHP() == 0)
		{
			monster->DestroyActor();
		}
		owner1->DestroyActor();
	}
	else if (type1 == focusMonster && type2 == bullet)
	{
		Pawn *monster = dynamic_cast<Pawn*>(owner1);
		monster->TakeDamage();
		if (monster->GetHP() == 0)
		{
			monster->DestroyActor();
		}
		owner2->DestroyActor();
	}


}

ECollideResult collisionOneWayCheck(CollisionObject &i_object1, CollisionObject &i_object2, Matrix &i_matrix1To2, const Vector3 &i_relativeVel1In2, float i_tickTime)
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

	ECollideResult XType, YType;
	if (timeCloseX < 0.0f)
	{
		if (timeOpenX < 0.0f)
		{
			return ECollideResult::ENone;
		}
		else if (timeOpenX < i_tickTime)
		{
			XType = ECollideResult::ELeave;
		}
		else
		{
			XType = ECollideResult::EStay;
		}
	}
	else if (timeCloseX < i_tickTime)
	{
		if (timeOpenX < i_tickTime)
		{
			XType = ECollideResult::EPass;
		}
		else
		{
			XType = ECollideResult::EEnter;
		}
	}
	else
	{
		return ECollideResult::ENone;
	}

	if (timeCloseY < 0.0f)
	{
		if (timeOpenY < 0.0f)
		{
			return ECollideResult::ENone;
		}
		else if (timeOpenY < i_tickTime)
		{
			YType = ECollideResult::ELeave;
		}
		else
		{
			YType = ECollideResult::EStay;
		}
	}
	else if (timeCloseY < i_tickTime)
	{
		if (timeOpenY < i_tickTime)
		{
			YType = ECollideResult::EPass;
		}
		else
		{
			YType = ECollideResult::EEnter;
		}
	}
	else
	{
		return ECollideResult::ENone;
	}

	// Return collide type based on XType and YType
	if (XType == ECollideResult::EStay && YType == ECollideResult::EStay)
	{
		return ECollideResult::EStay;
	}
	else if ((XType == ECollideResult::EEnter && YType == ECollideResult::EEnter) ||
		(XType == ECollideResult::EEnter && YType == ECollideResult::EStay) || (XType == ECollideResult::EStay && YType == ECollideResult::EEnter))
	{
		if (timeCloseY < 0.f || (timeCloseY > 0.f && timeCloseX > timeCloseY))
		{
			SetCollisionRecord(i_object1, i_object2, EAxis::X, timeCloseX, i_tickTime);
			HardCodeCollisionEvents(i_object1, i_object2);
		}
		else if (timeCloseX < 0.f || (timeCloseX > 0.f && timeCloseY > timeCloseX))
		{
			SetCollisionRecord(i_object1, i_object2, EAxis::Y, timeCloseY, i_tickTime);
			HardCodeCollisionEvents(i_object1, i_object2);
		}
	
		return ECollideResult::EEnter;
	}
	else if ((XType == ECollideResult::ELeave && YType == ECollideResult::ELeave) ||
		(XType == ECollideResult::ELeave && YType == ECollideResult::EStay) || (XType == ECollideResult::EStay && YType == ECollideResult::ELeave))
	{
		return ECollideResult::ELeave;
	}
	else if ((XType == ECollideResult::EEnter && (YType == ECollideResult::ELeave || YType == ECollideResult::EPass) && timeCloseX < timeOpenY) || ((XType == ECollideResult::ELeave || XType == ECollideResult::EPass) && YType == ECollideResult::EEnter && timeCloseY < timeOpenX) ||
		(XType == ECollideResult::ELeave && YType == ECollideResult::EPass && timeOpenX > timeCloseY) || (XType == ECollideResult::EPass && YType == ECollideResult::ELeave && timeOpenY > timeCloseX) ||
		(XType == ECollideResult::EStay && YType == ECollideResult::EPass) || (XType == ECollideResult::EPass && YType == ECollideResult::EStay) ||
		(XType == ECollideResult::EPass && YType == ECollideResult::EPass && timeOpenX > timeCloseY && timeOpenY > timeCloseX))
	{
		if (timeCloseY < 0.f || (timeCloseY > 0.f && timeCloseX > timeCloseY))
		{
			SetCollisionRecord(i_object1, i_object2, EAxis::X, timeCloseX, i_tickTime);
			HardCodeCollisionEvents(i_object1, i_object2);
		}
		else if (timeCloseX < 0.f || (timeCloseX > 0.f && timeCloseY > timeCloseX))
		{
			SetCollisionRecord(i_object1, i_object2, EAxis::Y, timeCloseY, i_tickTime);
			HardCodeCollisionEvents(i_object1, i_object2);
		}
		return ECollideResult::EPass;
	}
	return ECollideResult::ENone;
}

void CollisionObject::handleCollisionWith(CollisionObject& i_other)
{
	if (!i_other.active_) return;

	// precalculation, most actors were kick out by precalculation
	float tickTime = RealTimeManager::GetInstance()->GetLastFrameTimeS();

	static const float additionalCheckDistance = 5.f;

	float actualDistance = Vector2::Distance(Vector2(gameObject_->GetLocation()) + gameObject_->GetBoundingBox().GetCenter(), Vector2(i_other.gameObject_->GetLocation()) + i_other.gameObject_->GetBoundingBox().GetCenter());
	float maxCollideDistance = (gameObject_->GetVelocity() - i_other.gameObject_->GetVelocity()).Length() * tickTime + gameObject_->GetBoundingBox().GetExtend().Length() + i_other.gameObject_->GetBoundingBox().GetExtend().Length();
	/*float substract = Vector3::Distance(gameObject_->GetLocation() + Vector3(gameObject_->GetBoundingBox().GetCenter()), i_other.gameObject_->GetLocation() + Vector3(i_other.gameObject_->GetBoundingBox().GetCenter())) -
		(gameObject_->GetVelocity() - i_other.gameObject_->GetVelocity()).Length() * tickTime - gameObject_->GetBoundingBox().GetExtend().Length() - i_other.gameObject_->GetBoundingBox().GetExtend().Length();
	*/
	float subtract = actualDistance - maxCollideDistance;

	if (subtract > additionalCheckDistance)
	{
		return;
	}

	//double check collision betwwen this and i_other, content from lecture 8
	Matrix thisToOther = gameObject_->GetObjectToWorld() * i_other.gameObject_->GetWorldToObject();
	Vector3 relativeVelThisInOther = Vector3(Vector4(gameObject_->GetVelocity() - i_other.gameObject_->GetVelocity(), 0.f).Mul(i_other.gameObject_->GetWorldToObject()));
	ECollideResult thisWithOther = collisionOneWayCheck(*this, i_other, thisToOther, relativeVelThisInOther, tickTime);
	if (thisWithOther == ECollideResult::ENone) return;

	/*Matrix otherToThis = i_other.gameObject_->GetObjectToWorld() * gameObject_->GetWorldToObject();
	Vector3 relativeVelOtherInThis = Vector3(Vector4(i_other.gameObject_->GetVelocity() - gameObject_->GetVelocity(), 0.f).Mul(gameObject_->GetWorldToObject()));
	ECollideResult otherWithThis = collisionOneWayCheck(i_other, *this, otherToThis, relativeVelOtherInThis, tickTime, outTime2);

	if (otherWithThis == ECollideResult::ENone) return;*/

	//ASSERT(!((thisWithOther == ECollideResult::EEnter && otherWithThis == ECollideResult::ELeave) || (thisWithOther == ECollideResult::ELeave && otherWithThis == ECollideResult::EEnter)));

	ECollideResult commonRes;
	/*if ((thisWithOther == ECollideResult::EEnter && otherWithThis == ECollideResult::ELeave) || (thisWithOther == ECollideResult::ELeave && otherWithThis == ECollideResult::EEnter)) commonRes = ECollideResult::ENone;
	else if (thisWithOther == ECollideResult::EPass || otherWithThis == ECollideResult::EPass) commonRes = ECollideResult::EPass;
	else if (thisWithOther != ECollideResult::EStay) commonRes = thisWithOther;
	else commonRes = otherWithThis;*/

	commonRes = thisWithOther;

	ASSERT(gameObject_->GetOwner() != nullptr && i_other.gameObject_->GetOwner() != nullptr)
	if (commonRes == ECollideResult::EEnter)
	{
		DEBUG_PRINT("%s enter into %s", gameObject_->GetOwner()->GetName(), i_other.gameObject_->GetOwner()->GetName());
	}
	else if (commonRes == ECollideResult::ELeave)
	{
		DEBUG_PRINT("%s leave away %s", gameObject_->GetOwner()->GetName(), i_other.gameObject_->GetOwner()->GetName());
	}
	else if (commonRes == ECollideResult::EPass)
	{
		DEBUG_PRINT("%s pass through %s", gameObject_->GetOwner()->GetName(), i_other.gameObject_->GetOwner()->GetName());
	}
}




void CollisionObject::DoCollision(size_t i_index, Array<CollisionElement> &i_collisionElements)
{
	size_t counter = i_collisionElements.Size();
	if (active_ && GetGameObject()->GetActive())
	{
		// check forward, don't look back
		for (size_t i = i_index + 1; i < counter; ++i)
		{
			if (i_collisionElements[i].Pointer->IsValid())
			{
				handleCollisionWith(*i_collisionElements[i].Pointer);
			}
		}
	}
}

