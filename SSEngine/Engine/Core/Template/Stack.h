#pragma once

#include <vector>

#include "Core\Memory\New.h"
#include "Core\Basic\BasicFunctions.h"


template <typename T> class Stack {
	FORCEINLINE Stack();
	FORCEINLINE Stack(const Stack<T> &i_other);
	FORCEINLINE Stack(Stack<T> &&i_other);
	inline ~Stack();

	FORCEINLINE Stack<T>& operator=(const Stack<T> &i_other);
	FORCEINLINE Stack<T>& operator=(Stack<T> &&i_other);

	FORCEINLINE const T& Top() const;
	FORCEINLINE const T& Top(size_t i_index) const;
	FORCEINLINE void Push(const T& i_value);
	FORCEINLINE void Pop();
	FORCEINLINE bool Empty() const;
	FORCEINLINE size_t Size() const;

private:
	std::vector<T> elements_;
};







// implement inline

template <typename T> FORCEINLINE Stack<T>::Stack()
{
}

template <typename T> FORCEINLINE Stack<T>::Stack(const Stack<T> &i_other)
	: elements_(i_other.elements_)
{
}

template <typename T> FORCEINLINE Stack<T>::Stack(Stack<T> &&i_other)
{
	Basic::Swap(elements_, i_other.elements_);
}

template <typename T> inline Stack<T>::~Stack()
{
	elements_.clear();
}

template <typename T> FORCEINLINE Stack<T>& Stack<T>::operator=(const Stack<T> &i_other)
{
	elements_ = i_other.elements_;
	return *this;
}

template <typename T> FORCEINLINE Stack<T>& Stack<T>::operator=(Stack<T> &&i_other)
{
	Basic::Swap(elements_, i_other.elements_);
	return *this;
}

template <typename T> FORCEINLINE const T& Stack<T>::Top() const
{
	return elements_.back();
}

template <typename T> FORCEINLINE const T& Stack<T>::Top(size_t i_index) const
{
	ASSERT(-i_index <= elements_.size());
	return elements_[elements_.size() + i_index];
}

template <typename T> FORCEINLINE void Stack<T>::Push(const T& i_value)
{
	elements_.push_back(i_value);
}

template <typename T> FORCEINLINE void Stack<T>::Pop()
{
	elements_.pop_back();
}

template <typename T> FORCEINLINE bool Stack<T>::Empty() const
{
	return elements_.empty();
}

template <typename T> FORCEINLINE size_t Stack<T>::Size() const
{
	return elements_.size();
}
