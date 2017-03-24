#include "SubSystem\RenderManager.h"

void RenderManager::RenderUpdate() const
{
	GLib::BeginRendering();
	GLib::Sprites::BeginRendering();

	LinkedListNode<StrongPtr<RenderObject>> *temp = renderObjectList_.Head();
	while (temp != nullptr)
	{
		temp->Data->DoRender();
		temp = temp->Next;
	}

	GLib::Sprites::EndRendering();
	GLib::EndRendering();
}

StrongPtr<RenderObject> &RenderManager::AddToRenderList(RenderObject *i_renderObject)
{
	ASSERT(i_renderObject != nullptr);

	StrongPtr<RenderObject> *newRenderObject = new StrongPtr<RenderObject>(i_renderObject);
	renderObjectList_.PushHead(*newRenderObject);
	return *newRenderObject;
}

void RenderManager::RemoveFromList(StrongPtr<RenderObject> &i_renderObject)
{
	LinkedListNode<StrongPtr<RenderObject>> *temp = renderObjectList_.Head();
	if (temp == nullptr) return;

	if (temp->Data == i_renderObject)
	{
		delete &(temp->Data);
		renderObjectList_.PopHead();
	}
	else
	{
		while (temp->Next != nullptr)
		{
			if (temp->Next->Data == i_renderObject)
			{
				delete &(temp->Next->Data);
				renderObjectList_.DeleteNext(temp);
			}
			temp = temp->Next;
		}
	}
}

