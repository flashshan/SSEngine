#pragma once

#include "Core\Memory\New.h"
#include <vector>

// circle queue store data in vector


template<typename T> class CircleQueue {
public:
	FORCEINLINE CircleQueue(int32 i_size);
	FORCEINLINE void Push(const T& i_val);
	FORCEINLINE void Pop();

	FORCEINLINE bool Empty() const;

	FORCEINLINE const T& Head() const;
	FORCEINLINE const T& Tail() const;
	FORCEINLINE void Clear();

private:
	// don't support copy
	FORCEINLINE CircleQueue(CircleQueue<T> &i_other) {}
	FORCEINLINE CircleQueue<T>& operator=(CircleQueue<T> &i_other) {}

private:
	std::vector<T> elements_;
	int32 head_;
	int32 tail_;
	int32 size_;
};









// implement inline

template<typename T> FORCEINLINE CircleQueue<T>::CircleQueue(int32 i_size)
	: head_(i_size - 1), tail_(0), size_(0)
{
	ASSERT(i_size > 0);
	elements_.resize(i_size);
}

template<typename T> FORCEINLINE void CircleQueue<T>::Push(const T &i_val)
{
	head_ = (++head_) % elements_.size();
	elements_[head_] = i_val;
	if (size_ == elements_.size())
	{
		tail_ = (++tail_) % size_;
	}
	else
	{
		++size_;
	}
}

template<typename T> FORCEINLINE void CircleQueue<T>::Pop()
{
	if (size_ > 0)
	{
		tail_ = (++tail_) % elements_.size();
		--size_;
	}
}

template<typename T> FORCEINLINE bool CircleQueue<T>::Empty() const
{
	return size_ == 0;
}

template<typename T> FORCEINLINE const T& CircleQueue<T>::Head() const
{
	return elements_[head_];
}

template<typename T> FORCEINLINE const T& CircleQueue<T>::Tail() const
{
	return elements_[tail_];
}

template<typename T> FORCEINLINE void CircleQueue<T>::Clear()
{
	size_ = 0;
	head_ = 0;
	tail_ = 0;
}




