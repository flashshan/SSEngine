#pragma once

#include "Core\Template\List.h"
#include "RenderObject.h"


class RenderManager
{
public:
	static FORCEINLINE RenderManager *GetInstance();

	void RenderUpdate() const;
	StrongPtr<RenderObject> &AddToRenderList(RenderObject *i_renderObject);
	void RemoveFromList(StrongPtr<RenderObject> &i_renderObject);

private:
	FORCEINLINE RenderManager();

private:
	LinkedList<StrongPtr<RenderObject>> renderObjectList_;
};






// implement

FORCEINLINE RenderManager *RenderManager::GetInstance()
{
	static RenderManager *globalInstance;
	if (globalInstance == nullptr)
	{
		globalInstance = new RenderManager();
	}
	return globalInstance;
}


FORCEINLINE RenderManager::RenderManager()
{
}