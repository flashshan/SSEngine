#include "SubSystem\PhysicsObject.h"

#include "Manager\RealTimeManager.h"


void PhysicsObject::UpdatePhysics()
{
	if (gameObject_)
	{
		ASSERT(!Float::IsZero(mass_));
		Vector3 currentVelocity = (*gameObject_).GetVelocity();
		Vector3 accelerate = (force_ + (-currentVelocity) * currentVelocity.Length() * drag_) / mass_;;
		(*gameObject_).SetVelocity(currentVelocity + accelerate * RealTimeManager::GetInstance()->GetLastFrameTimeS());
		force_ = Vector3(0.0f, 0.0f, 0.0f);
	}
	else
	{
		RemovePhysicsObject();
	}
}