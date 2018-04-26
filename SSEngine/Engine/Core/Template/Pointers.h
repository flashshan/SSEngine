#pragma once

#include "Core\Basic\BasicFunctions.h"
#include "Core\Memory\New.h"

struct Counter {
	uint32 StrongCounter;
	uint32 WeakCounter;
	FORCEINLINE Counter(uint32 i_strongCount, uint32 i_weakCount = 0);
};

template<typename T> class StrongPtr
{
public:
	FORCEINLINE StrongPtr(T * i_ptr = nullptr, Counter *i_counter = nullptr);
	FORCEINLINE StrongPtr(const StrongPtr<T> &i_other);
	FORCEINLINE StrongPtr(StrongPtr<T> &&i_other);
	FORCEINLINE ~StrongPtr();

	FORCEINLINE StrongPtr<T>& operator=(const StrongPtr<T> &i_other);
	FORCEINLINE StrongPtr<T>& operator=(StrongPtr<T> &&i_other);

	FORCEINLINE Counter *GetCounter() const { return counter_; }

	FORCEINLINE void ClearPointer();

	FORCEINLINE T & operator*() const { return *object_; }
	FORCEINLINE T * operator->() const { return object_; }   
	FORCEINLINE bool operator<(const StrongPtr<T>& i_other) const { return *object_ < *i_other; }

	FORCEINLINE operator bool() const { return object_ != nullptr; }
	FORCEINLINE bool operator==(const StrongPtr<T> & i_other) const { return object_ == i_other.object_ && counter_ == i_other.counter_; }
	FORCEINLINE bool operator==(T *i_ptr) const { return object_ == i_ptr; }

	FORCEINLINE void Prefetch() const;
private:
	T *object_;
	Counter *counter_;
};

template<typename T>
class WeakPtr
{
public:
	FORCEINLINE WeakPtr();
	FORCEINLINE WeakPtr(const StrongPtr<T> &i_other);
	FORCEINLINE WeakPtr(const WeakPtr<T> &i_other);
	FORCEINLINE WeakPtr(WeakPtr<T> &&i_other);
	FORCEINLINE ~WeakPtr();
	
	FORCEINLINE WeakPtr<T>& operator=(const WeakPtr<T> &i_other);
	FORCEINLINE WeakPtr<T>& operator=(WeakPtr<T> &&i_other);
	FORCEINLINE WeakPtr<T>& operator=(const StrongPtr<T> &i_other);

	FORCEINLINE StrongPtr<T> Acquire() const;
	
	FORCEINLINE void ClearPointer();
	
	FORCEINLINE T & operator*() const { return *Acquire(); }
	FORCEINLINE T * operator->() const { return Acquire().operator->(); }
	FORCEINLINE bool operator<(const WeakPtr<T>& i_other) const { return *object_ < *i_other; }

	FORCEINLINE operator bool() const { return (object_ != nullptr && counter_ != nullptr && counter_->StrongCounter > 0); }
	FORCEINLINE bool operator==(const WeakPtr<T> &i_other) const { return object_ == i_other.object_ && counter_ == i_other.counter_; }
	FORCEINLINE bool operator==(T * i_ptr) const { return object_ == i_ptr; }

	FORCEINLINE void Prefetch() const;
private:
	T *object_;
	Counter *counter_;
};


template<typename T>
class UniquePtr
{
	// TO DO
public:
	explicit FORCEINLINE UniquePtr(T *i_ptr = nullptr);
	FORCEINLINE UniquePtr(UniquePtr<T> &i_other);
	FORCEINLINE ~UniquePtr();

	FORCEINLINE void operator =(UniquePtr<T> &i_other);

	FORCEINLINE T & operator*() const { return *object_; }
	FORCEINLINE T * operator->() const { return object_; }   // TO TEST

	FORCEINLINE operator bool() const { return object_ == nullptr; }

	FORCEINLINE void Prefetch() const; 
private:
	T * object_;
};







// type cast for Strong pointer
template<typename From, typename To> 
FORCEINLINE StrongPtr<To> CastStrongPtr(StrongPtr<From> &i_from)
{
	if (i_from)
	{
		return StrongPtr<To>(reinterpret_cast<To*>(&(*i_from)), i_from.GetCounter());
	}
	else
	{
		return StrongPtr<To>(nullptr);
	}
}

//type cast for weak pointer
template<typename From, typename To>
FORCEINLINE WeakPtr<To> CastWeakPtr(WeakPtr<From> &i_from)
{
	if (i_from)
	{
		return WeakPtr<To>(CastStrongPtr<From, To>(i_from.Acquire()));
	}
	else
	{
		return WeakPtr<To>();
	}
}










// implement inline

FORCEINLINE Counter::Counter(uint32 i_strongCount, uint32 i_weakCount)
	: StrongCounter(i_strongCount), WeakCounter(i_weakCount) 
{
}




template<typename T> FORCEINLINE StrongPtr<T>::StrongPtr(T *i_ptr, Counter *i_counter)
	: object_(i_ptr), counter_(i_counter)
{
	if (i_ptr == nullptr)
	{
		ASSERT(i_counter == nullptr);
	}
	else 
	{
		if (i_counter == nullptr)
		{
			counter_ = new TRACK_NEW Counter(1);
		}
		else
		{
			++counter_->StrongCounter;
		}
	}
}

template<typename T> FORCEINLINE StrongPtr<T>::StrongPtr(const StrongPtr<T> &i_other)
	: object_(i_other.object_), counter_(i_other.counter_)
{
	if (object_ != nullptr)
	{
		++counter_->StrongCounter;
	}
}

template<typename T> FORCEINLINE StrongPtr<T>::StrongPtr(StrongPtr<T> &&i_other)
	: object_(nullptr), counter_(nullptr)
{
	Basic::Swap(object_, i_other.object_);
	Basic::Swap(counter_, i_other.counter_);
}

template<typename T> FORCEINLINE void StrongPtr<T>::ClearPointer()
{
	if (object_ != nullptr)
	{
		if (--(counter_->StrongCounter) == 0)
		{
			delete object_;
			if (counter_->WeakCounter == 0)
			{
				delete counter_;
			}
		}
	}
	object_ = nullptr;
	counter_ = nullptr;
}

template<typename T> FORCEINLINE StrongPtr<T>::~StrongPtr()
{
	ClearPointer();
}

template<typename T> FORCEINLINE StrongPtr<T>& StrongPtr<T>::operator=(const StrongPtr<T> & i_other)
{
	if (object_ == i_other.object_) return *this;

	ClearPointer();
	object_ = i_other.object_;
	counter_ = i_other.counter_;

	if (object_ != nullptr)
	{
		++counter_->StrongCounter;
	}
	return *this;
}

template<typename T> FORCEINLINE StrongPtr<T>& StrongPtr<T>::operator=(StrongPtr<T> &&i_other)
{
	Basic::Swap(object_, i_other.object_);
	Basic::Swap(counter_, i_other.counter_);

	i_other.ClearPointer();
	return *this;
}

template<typename T> FORCEINLINE void StrongPtr<T>::Prefetch() const
{
	_mm_prefetch(object_, _MM_HINT_T0);
}




template<typename T> FORCEINLINE WeakPtr<T>::WeakPtr() 
	: object_(nullptr), counter_(nullptr)
{
}

template<typename T> FORCEINLINE WeakPtr<T>::WeakPtr(const StrongPtr<T> &i_other)
	: object_(nullptr), counter_(nullptr)
{
	if (i_other)
	{
		object_ = &(*i_other);
		counter_ = i_other.GetCounter();

		++counter_->WeakCounter;
	}
}

template<typename T> FORCEINLINE WeakPtr<T>::WeakPtr(const WeakPtr<T> &i_other)
	: object_(nullptr), counter_(nullptr)
{
	if (i_other)
	{
		object_ = i_other.object_;
		counter_ = i_other.counter_;
		++counter_->WeakCounter;
	}
}

template<typename T> FORCEINLINE WeakPtr<T>::WeakPtr(WeakPtr<T> && i_other)
	: object_(nullptr), counter_(nullptr)
{
	Basic::Swap(object_, i_other.object_);
	Basic::Swap(counter_, i_other.counter_);
}


template<typename T> FORCEINLINE void WeakPtr<T>::ClearPointer()
{
	object_ = nullptr;
	if(counter_ != nullptr)
	{
		if ((--counter_->WeakCounter) == 0 && counter_->StrongCounter == 0)
		{
			delete counter_;
		}
	}
	counter_ = nullptr;
}

template<typename T> FORCEINLINE WeakPtr<T>::~WeakPtr()
{
	ClearPointer();
}

template<typename T> FORCEINLINE WeakPtr<T>& WeakPtr<T>::operator=(const WeakPtr<T> & i_other)
{
	if (i_other)
	{
		if (object_ == i_other.object_) return *this;

		ClearPointer();
		object_ = i_other.object_;
		counter_ = i_other.counter_;
		++counter_->WeakCounter;
	}
	else
	{
		object_ = nullptr;
		counter_ = nullptr;
	}
	return *this;
}

template<typename T> FORCEINLINE WeakPtr<T>& WeakPtr<T>::operator=(const StrongPtr<T> &i_other)
{
	ClearPointer();

	if (i_other)
	{
		object_ = &(*i_other);
		counter_ = i_other.GetCounter();

		++counter_->WeakCounter;
	}
	return *this;
}


template<typename T> FORCEINLINE WeakPtr<T>& WeakPtr<T>::operator=(WeakPtr<T> &&i_other)
{
	Basic::Swap(object_, i_other.object_);
	Basic::Swap(counter_, i_other.counter_);

	i_other.ClearPointer();
	return *this;
}

template<typename T> FORCEINLINE StrongPtr<T> WeakPtr<T>::Acquire() const
{
	if (*this)
	{
		return StrongPtr<T>(object_, counter_);
	}
	else
	{
		return StrongPtr<T>(nullptr);
	}
}

template<typename T> FORCEINLINE void WeakPtr<T>::Prefetch() const
{
	_mm_prefetch(object_, _MM_HINT_T0);
}






template<typename T> FORCEINLINE UniquePtr<T>::UniquePtr(T * i_ptr)
	: object_(i_ptr)
{
}

template<typename T> FORCEINLINE UniquePtr<T>::UniquePtr(UniquePtr<T> &i_other)
	: object_(i_other.object_)
{
	i_other.object_ = nullptr;
}

template<typename T> FORCEINLINE UniquePtr<T>::~UniquePtr()
{
	if (object_ != nullptr)
	{
		delete object_;
	}
}

template<typename T> FORCEINLINE void UniquePtr<T>::operator =(UniquePtr<T> &i_other)
{
	object_ = i_other.object_;
	i_other.object_ = nullptr;
}

template<typename T> FORCEINLINE void UniquePtr<T>::Prefetch() const
{
	_mm_prefetch(object_, _MM_HINT_T0);
}