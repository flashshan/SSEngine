#pragma once

#include "Core\Memory\New.h"
#include "Core\Basic\BasicFunctions.h"
#include <vector>

// a encapsulation of std::vector, in order to customize API

template<typename T> class Array {
public:
	FORCEINLINE Array();
	FORCEINLINE Array(size_t i_size);
	FORCEINLINE Array(size_t i_size, const T& i_value);
	FORCEINLINE Array(const Array<T> &i_other);
	FORCEINLINE Array(Array<T> &&i_other);
	inline ~Array();

	FORCEINLINE Array<T>& operator=(const Array<T>& i_other);
	FORCEINLINE Array<T>& operator=(Array<T>&& i_other);

	FORCEINLINE void Reserve(size_t i_size);
	FORCEINLINE void Resize(size_t i_size);
	FORCEINLINE void Resize(size_t i_size, const T& i_value);
	FORCEINLINE void Add(const T& i_value);
	FORCEINLINE void Add(const T& i_value, size_t i_index);
	FORCEINLINE void Remove();
	FORCEINLINE void Remove(size_t i_index);
	FORCEINLINE void NoOrderRemove(size_t i_index);
	
	FORCEINLINE const T& Back() const;
	FORCEINLINE size_t Size() const;
	FORCEINLINE bool Empty() const;
	FORCEINLINE void Clear();

	FORCEINLINE T& operator[](size_t i_index);
private:
	std::vector<T> elements_;
};









// implement inline

template <typename T> FORCEINLINE Array<T>::Array()
{
}

template <typename T> FORCEINLINE Array<T>::Array(size_t i_size)
	: elements_(i_size)
{
}

template <typename T> FORCEINLINE Array<T>::Array(size_t i_size, const T& i_value)
	: elements_(i_size, i_value)
{
}

template <typename T> FORCEINLINE Array<T>::Array(const Array<T> &i_other)
	: elements_(i_other.elements_)
{
}

template <typename T> FORCEINLINE Array<T>::Array(Array<T> &&i_other)
{
	Basic::Swap(elements_, i_other.elements_);
}

template <typename T> inline Array<T>::~Array()
{
	elements_.clear();
}

template <typename T> FORCEINLINE Array<T>& Array<T>::operator=(const Array<T>& i_other)
{
	elements_ = i_other.elements_;
	return *this;
}

template <typename T> FORCEINLINE Array<T>& Array<T>::operator=(Array<T>&& i_other)
{
	Basic::Swap(elements_, i_other.elements_);
	return *this;
}

template <typename T> FORCEINLINE void Array<T>::Reserve(size_t i_size)
{
	elements_.reserve(i_size);
}

template <typename T> FORCEINLINE void Array<T>::Resize(size_t i_size)
{
	elements_.resize(i_size);
}

template <typename T> FORCEINLINE void Array<T>::Resize(size_t i_size, const T& i_value)
{
	elements_.resize(i_size, i_value);
}

template <typename T> FORCEINLINE void Array<T>::Add(const T& i_value)
{
	elements_.push_back(i_value);
}

template <typename T> FORCEINLINE void Array<T>::Add(const T& i_value, size_t i_index)
{
	elements_.insert(elements_.begin() + i_index, i_value);
}

template <typename T> FORCEINLINE void Array<T>::Remove()
{
	elements_.pop_back();
}

template <typename T> FORCEINLINE void Array<T>::Remove(size_t i_index)
{
	elements_.erase(elements_.begin() + i_index);
}

template <typename T> FORCEINLINE void Array<T>::NoOrderRemove(size_t i_index)
{
	ASSERT(i_index < elements_.size());
	Basic::Swap(elements_[i_index], elements_.back());
	elements_.pop_back();
}

template <typename T> FORCEINLINE const T& Array<T>::Back() const
{
	return elements_.back();
}


template <typename T> FORCEINLINE size_t Array<T>::Size() const
{
	return elements_.size();
}

template <typename T> FORCEINLINE bool Array<T>::Empty() const
{
	return elements_.empty();
}

template <typename T> FORCEINLINE void Array<T>::Clear()
{
	elements_.clear();
}

template <typename T>FORCEINLINE T& Array<T>::operator[](size_t i_index)
{
	return elements_[i_index];
}
