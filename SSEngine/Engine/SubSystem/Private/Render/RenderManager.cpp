#include "SubSystem\Render\RenderManager.h"



RenderManager *RenderManager::globalInstance_ = nullptr;

void RenderManager::RenderUpdate()
{
	GLib::BeginRendering();
	GLib::Sprites::BeginRendering();
	
	for(auto it = renderElements_.begin(); it != renderElements_.end(); ++it)
	{
		if ((*it).Pointer->IsValid())
		{
			(*it).Pointer->DoRender();
		}
		else
		{
			renderElements_.erase(it);
		}
	}

	GLib::Sprites::EndRendering();
	GLib::EndRendering();
}


// go through the set, seldomly used
void RenderManager::Remove(const WeakPtr<RenderObject> &i_renderObject)
{
	EnterCriticalSection(&criticalSection_);
	for (auto && item : renderElements_)
	{
		if (&(*item.Pointer) == &(*i_renderObject))
		{
			renderElements_.erase(item);
			break;
		}
	}
	LeaveCriticalSection(&criticalSection_);
}
