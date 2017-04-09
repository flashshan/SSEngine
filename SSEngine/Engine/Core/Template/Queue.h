#pragma once

#include "Core\Memory\New.h"
#include "Core\Basic\BasicFunctions.h"
#include <queue>

// a encapsulation of std::queue, in order to customize API

template <typename T> class Queue
{
public:
	FORCEINLINE Queue();
	FORCEINLINE Queue(const Queue<T>& i_other);
	FORCEINLINE Queue(Queue<T> &&i_other);
	inline ~Queue();
	FORCEINLINE Queue<T>& operator=(const Queue<T>& i_other);
	FORCEINLINE Queue<T>& operator=(Queue<T>&& i_other);

	FORCEINLINE const T& Front() const;
	FORCEINLINE const T& Back() const;
	FORCEINLINE void Push(const T& i_value);
	FORCEINLINE void Pop();

	FORCEINLINE bool Empty() const;
	FORCEINLINE size_t Size() const;

private:
	std::queue<T> queue_;
};








// implement inline

template <typename T> FORCEINLINE Queue<T>::Queue()
{	
}

template <typename T> FORCEINLINE Queue<T>::Queue(const Queue<T>& i_other)
	: queue_(i_other.queue_)
{
}

template <typename T> FORCEINLINE Queue<T>::Queue(Queue<T> &&i_other)
{
	Basic::Swap(queue_, i_other.queue_);
}

template <typename T> inline Queue<T>::~Queue()
{	
}

template <typename T> FORCEINLINE Queue<T>& Queue<T>::operator=(const Queue<T>& i_other)
{
	queue_ = i_other.queue_;
	return *this;
}

template <typename T> FORCEINLINE Queue<T>& Queue<T>::operator=(Queue<T>&& i_other)
{
	Basic::Swap(queue_, i_other.queue_);
	return *this;
}

template <typename T> FORCEINLINE const T& Queue<T>::Front() const
{
	return queue_.front();
}

template <typename T> FORCEINLINE const T& Queue<T>::Back() const
{
	return queue_.back();
}

template <typename T> FORCEINLINE void Queue<T>::Push(const T& i_value)
{
	queue_.push(i_value);
}

template <typename T> FORCEINLINE void Queue<T>::Pop()
{
	queue_.pop();
}

template <typename T> FORCEINLINE bool Queue<T>::Empty() const
{
	return queue_.empty();
}

template <typename T> FORCEINLINE size_t Queue<T>::Size() const
{
	return queue_.size();
}