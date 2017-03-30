#include "Object\Actor.h"

Actor::~Actor()
{
}

void Actor::Update()
{
	(*gameObject_).Update();
}
