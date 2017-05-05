#pragma once

#include "IMessage.h"
#include "Core\CoreMinimal.h"
#include "Core\Math\Vector3.h"

class CollisionMessage : IMessage
{
public:
	FORCEINLINE CollisionMessage(const Vector3 &i_vector, float i_collideTime);

	virtual	~CollisionMessage() {}

private:
	Vector3 newVelocity_;
	float collideTime_;
};

FORCEINLINE CollisionMessage::CollisionMessage(const Vector3 &i_vector, float i_collideTime)
	: newVelocity_(i_vector), collideTime_(i_collideTime)
{
	type_ = MessageType::ECollision;
}
