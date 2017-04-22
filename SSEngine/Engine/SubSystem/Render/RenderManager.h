#pragma once

#include <set>

#include "Core\Memory\New.h"
#include "RenderObject.h"


// hold a strongPointer and a RenderObject it point to, for cache cohorency
struct RenderElement {
	explicit FORCEINLINE RenderElement(const RenderObject &i_renderObject);
	FORCEINLINE RenderElement(const RenderElement &i_other);
	FORCEINLINE RenderElement &operator=(const RenderElement &i_other);
	FORCEINLINE bool operator<(const RenderElement &i_other) const;

private:
	RenderObject Object;
public:
	StrongPtr<RenderObject> Pointer;
};



//singleton Mananger class
class RenderManager
{
public:
	static FORCEINLINE RenderManager *CreateInstance();
	static FORCEINLINE RenderManager *GetInstance();
	static FORCEINLINE void DestroyInstance();
	inline ~RenderManager();

	void RenderUpdate();
	FORCEINLINE WeakPtr<RenderObject> AddRenderObject(const RenderObject &i_renderObject);
	void Remove(const WeakPtr<RenderObject> &i_renderObject);

private:
	FORCEINLINE RenderManager();
	FORCEINLINE RenderManager(RenderManager &i_other) {}
	FORCEINLINE RenderManager& operator=(RenderManager &i_other) {}

	static RenderManager *globalInstance_;

private:
	// multiset to support order for priority
	std::multiset<RenderElement> renderElements_;

	CRITICAL_SECTION criticalSection_;
};






// implement forceinline

FORCEINLINE RenderElement::RenderElement(const RenderObject &i_renderObject)
	: Object(i_renderObject), Pointer(&Object)
{
}


FORCEINLINE RenderElement::RenderElement(const RenderElement &i_other)
	: Object(i_other.Object), Pointer(&Object)
{
}

FORCEINLINE RenderElement &RenderElement::operator=(const RenderElement &i_other)
{
	Object = i_other.Object;
	Pointer = &Object;
	return *this;
}

FORCEINLINE bool RenderElement::operator<(const RenderElement &i_other) const
{
	return Pointer->GetPriority() < i_other.Pointer->GetPriority();
}





FORCEINLINE RenderManager *RenderManager::CreateInstance()
{
	ASSERT(RenderManager::globalInstance_ == nullptr);
	RenderManager::globalInstance_ = new TRACK_NEW RenderManager();
	return RenderManager::globalInstance_;
}

FORCEINLINE RenderManager *RenderManager::GetInstance()
{
	ASSERT(RenderManager::globalInstance_ != nullptr);
	return RenderManager::globalInstance_;
}

FORCEINLINE void RenderManager::DestroyInstance()
{
	ASSERT(RenderManager::globalInstance_ != nullptr);
	delete RenderManager::globalInstance_;
	RenderManager::globalInstance_ = nullptr;
}


FORCEINLINE RenderManager::RenderManager()
{
	InitializeCriticalSection(&criticalSection_);
}

inline RenderManager::~RenderManager()
{
	renderElements_.clear();
}

FORCEINLINE WeakPtr<RenderObject> RenderManager::AddRenderObject(const RenderObject &i_renderObject)
{
	EnterCriticalSection(&criticalSection_);
	RenderElement tempElement(i_renderObject);
	renderElements_.insert(tempElement);
	LeaveCriticalSection(&criticalSection_);

	auto it = renderElements_.find(tempElement);
	ASSERT(it != renderElements_.end());
	return WeakPtr<RenderObject>(it->Pointer);
}

