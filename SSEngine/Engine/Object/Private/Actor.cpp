#include "Object\Actor.h"

Actor::~Actor()
{
	delete name_;
}

void Actor::Update()
{
	(*gameObject_).Update();
}
