#include "SubSystem\Physics\PhysicsObject.h"

#include "Manager\RealTimeManager.h"


void PhysicsObject::DoPhysics()
{
	if (gameObject_ && gameObject_->GetActive())
	{
		ASSERT(!Float::IsZero(mass_));

		//gameObject_.Prefetch();
		Vector3 currentVelocity = (*gameObject_).GetVelocity();
		Vector3 accelerate = (force_ + (-currentVelocity) * currentVelocity.Length() * drag_) / mass_;
		//Vector3 accelerate = (force_ + (-currentVelocity) * drag_) / mass_;

		(*gameObject_).SetVelocity(currentVelocity + accelerate * RealTimeManager::GetInstance()->GetLastFrameTimeS());
		force_ = Vector3(0.0f, 0.0f, 0.0f);
	}
}