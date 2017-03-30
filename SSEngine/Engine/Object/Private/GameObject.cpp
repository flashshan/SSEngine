#include "Object\GameObject.h"
#include "Manager\RealTimeManager.h"

void GameObject::Update()
{
	transform_.Translate(velocity_ * RealTimeManager::GetInstance()->GetLastFrameTimeS());
}