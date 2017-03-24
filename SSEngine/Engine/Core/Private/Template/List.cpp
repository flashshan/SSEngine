#include "Core\Template\List.h"

template<typename T> void LinkedList<T>::Clear()
{
	if (head_ == nullptr) return;

	LinkedListNode<T> temp;
	do 
	{
		temp = head_->Next;
		delete head_;
		head_ = temp;
	} while (head_ != nullptr);
}


template<typename T> void List<T>::Clear()
{
	if (head_ == nullptr) return;

	ListNode<T> temp;
	do
	{
		temp = head_->Next;
		delete head_;
		head_ = temp;
	} while (head_ != nullptr);
}