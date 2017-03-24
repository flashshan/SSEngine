#pragma once

#include "Core\Basic\BasicFunctions.h"

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

	FORCEINLINE void operator=(const StrongPtr<T> &i_other);
	FORCEINLINE void operator=(StrongPtr<T> &&i_other);

	FORCEINLINE Counter *GetCounter() const { return counter_; }

	FORCEINLINE void ClearPointer();

	FORCEINLINE T & operator*() const { return *object_; }
	FORCEINLINE T * operator->() const { return object_; }   

	FORCEINLINE operator bool() const { return object_ != nullptr; }
	FORCEINLINE bool operator==(const StrongPtr<T> & i_other) const { return object_ == i_other.object_; }

private:

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
	
	FORCEINLINE void operator=(const WeakPtr<T> &i_other);
	FORCEINLINE void operator=(WeakPtr<T> &&i_other);

	FORCEINLINE StrongPtr<T> Acquire() const;
	
	FORCEINLINE void ClearPointer();
	
	FORCEINLINE T & operator*() const { return *Aquire(); }
	FORCEINLINE T * operator->() const { return Aquire(); }

	FORCEINLINE operator bool() const { return (object_ != nullptr && counter_ != nullptr && counter_->StrongCounter > 0); }
	FORCEINLINE bool operator==(const WeakPtr<T> &i_other) const { return object_ == i_other.object_; }

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
private:
	T * object_;
};



// implement forceinline

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
			counter_ = new Counter(1);
		}
		else
		{
			++counter_->StrongCounter;
		}
	}
}

template<typename T> FORCEINLINE StrongPtr<T>::StrongPtr(const StrongPtr<T> &i_other)
{
	object_ = i_other.object_;
	counter_ = i_other.counter_;

	if (object_ != nullptr)
		++counter_->StrongCounter;
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
		object_ = nullptr;
		counter_ = nullptr;
	}
}

template<typename T> FORCEINLINE StrongPtr<T>::~StrongPtr()
{
	ClearPointer();
}

template<typename T> FORCEINLINE void StrongPtr<T>::operator=(const StrongPtr<T> & i_other)
{
	if (object_ == i_other.object_) return;

	ClearPointer();
	object_ = i_other.object_;
	counter_ = i_other.counter_;

	if (object_ != nullptr)
	{
		++counter_->StrongCounter;
	}
}

template<typename T> FORCEINLINE void StrongPtr<T>::operator=(StrongPtr<T> &&i_other)
{
	Basic::Swap(object_, i_other.object_);
	Basic::Swap(counter_, i_other.counter_);

	i_other.ClearPointer();
}





template<typename T> FORCEINLINE WeakPtr<T>::WeakPtr() 
	: object_(nullptr), counter_(nullptr)
{
}

template<typename T> FORCEINLINE WeakPtr<T>::WeakPtr(const StrongPtr<T> &i_other)
{
	object_ = &(*i_other);
	counter_ = i_other.GetCounter();

	if (object_ != nullptr)
		++counter_->WeakCounter;
}

template<typename T> FORCEINLINE WeakPtr<T>::WeakPtr(const WeakPtr<T> &i_other)
{
	if (i_other)
	{
		StrongPtr<T> tempStrongPointer = i_other.Acquire();
		object_ = &(*tempStrongPointer);
		counter_ = tempStrongPointer.GetCounter();
		++counter_->WeakCounter;
	}
}

template<typename T> FORCEINLINE WeakPtr<T>::WeakPtr(WeakPtr<T> && i_other)
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
			counter_ = nullptr;
		}
	}
}

template<typename T> FORCEINLINE WeakPtr<T>::~WeakPtr()
{
	ClearPointer();
}

template<typename T> FORCEINLINE void WeakPtr<T>::operator=(const WeakPtr<T> & i_other)
{
	if (i_other)
	{
		if (object_ == i_other.object_) return;

		ClearPointer();
		object_ = i_other.object_;
		counter_ = i_other.counter_;
		if (object_ != nullptr)
		{
			++counter_->weakCounter;
		}
	}
	else
	{
		object_ = nullptr;
		counter_ = nullptr;
	}
}

template<typename T> FORCEINLINE void WeakPtr<T>::operator=(WeakPtr<T> &&i_other)
{
	Basic::Swap(object_, i_other.object_);
	Basic::Swap(counter_, i_other.counter_);

	i_other.ClearPointer();
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








template<typename T> FORCEINLINE UniquePtr<T>::UniquePtr(T * i_ptr = nullptr)
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