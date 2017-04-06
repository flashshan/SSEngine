#include "SubSystem\PhysicsManager.h"


PhysicsManager *PhysicsManager::globalInstance_ = nullptr;

void PhysicsManager::PhysicsUpdate() const
{
	LinkedListNode<StrongPtr<PhysicsObject>> *temp = physicsObjectList_.Head();
	LinkedListNode<StrongPtr<PhysicsObject>> *recordNext;
	while (temp != nullptr)
	{
		recordNext = temp->Next;
		temp->Data->UpdatePhysics();
		temp = recordNext;
	}
}

StrongPtr<PhysicsObject>& PhysicsManager::AddPhysicsObject(const StrongPtr<GameObject> &i_gameObject, const float i_mass, const float i_drag)
{
	StrongPtr<PhysicsObject> newPhysicsObject = new TRACK_NEW PhysicsObject(i_gameObject, i_mass, i_drag);
	physicsObjectList_.PushHead(newPhysicsObject);
	return physicsObjectList_.Head()->Data;
}

void PhysicsManager::RemoveFromList(PhysicsObject &i_physicsObject)
{
	LinkedListNode<StrongPtr<PhysicsObject>> *temp = physicsObjectList_.Head();
	if (temp == nullptr) return;

	if (temp->Data == &i_physicsObject)
	{
		physicsObjectList_.PopHead();          // Pop include delete
	}
	else
	{
		while (temp->Next != nullptr)
		{
			if (temp->Next->Data == &i_physicsObject)
			{
				physicsObjectList_.DeleteNext(temp);
				break;
			}
			temp = temp->Next;
		}
	}
}



// for PhysicsObject
void PhysicsObject::RemovePhysicsObject()
{
	PhysicsManager::GetInstance()->RemoveFromList(*this);
}