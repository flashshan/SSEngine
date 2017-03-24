#pragma once

#include "Core\Basic\Assert.h"
#include "Core\Basic\BasicTypes.h"

// list is used for optimize insert and delete with order required vector
// list is slow because it has to dynamic new


template<typename T> struct LinkedListNode {
	T Data;
	LinkedListNode<T> *Next;
	FORCEINLINE LinkedListNode(const T &i_data, LinkedListNode *i_next = nullptr) : Data(i_data), Next(i_next) {}
	FORCEINLINE LinkedListNode(const LinkedListNode<T> &i_other) : Data(i_other.Data), Next(i_other.Next) {}
};

template<typename T> struct ListNode {
	T Data;
	ListNode<T> *Previous;
	ListNode<T> *Next;
	FORCEINLINE ListNode(const T &i_data, ListNode *i_previous = nullptr, ListNode *i_next = nullptr) : Data(i_data), Previous(i_previous), Next(i_next) {}
	FORCEINLINE ListNode(const ListNode<T> &i_other) : Data(i_other.Data), Previous(i_other.Previous), Next(i_other.Next) {}
};


template<typename T> class LinkedList {
public:
	FORCEINLINE LinkedList();
	FORCEINLINE LinkedListNode<T> *Head() const { return head_; }

	FORCEINLINE void PushHead(const T &i_val);
	FORCEINLINE void PopHead();

	FORCEINLINE void InsertNext(LinkedListNode<T> *i_pos, const T &i_val);
	FORCEINLINE void DeleteNext(LinkedListNode<T> *i_pos);

	void Clear();

private:
	LinkedListNode<T> *head_;
};

template<typename T> class List {
public:
	FORCEINLINE List();
	FORCEINLINE ListNode<T> *Head() const { return head_; }
	FORCEINLINE ListNode<T> *Tail() const { return tail_; }

	FORCEINLINE void PushHead(const T &i_node);
	FORCEINLINE void PopHead();
	FORCEINLINE void PushTail(const T &i_node);
	FORCEINLINE void PopTail();

	FORCEINLINE void InsertNext(ListNode<T> *i_pos, const T &i_node);
	FORCEINLINE void Delete(ListNode<T> *i_pos);
	
	void Clear();

private:
	ListNode<T> *head_;
	ListNode<T> *tail_;
};





// implement forceinline

template<typename T> FORCEINLINE LinkedList<T>::LinkedList()
	: head_(nullptr) 
{
}

template<typename T> FORCEINLINE void LinkedList<T>::PushHead(const T &i_val)
{
	LinkedListNode<T> *node = new LinkedListNode<T>(i_val, head_);
	head_ = node;
}

template<typename T> FORCEINLINE void LinkedList<T>::PopHead()
{
	if (head_ == nullptr) return;
	LinkedListNode<T> *temp = head_;
	head_ = head_->Next;
	delete temp;
}


template<typename T> FORCEINLINE void LinkedList<T>::InsertNext(LinkedListNode<T> *i_pos, const T &i_val)
{
	ASSERT(i_pos != nullptr);
	LinkedListNode<T> *node = new LinkedListNode<T>(i_val, i_pos->Next);
	i_pos->Next = node;
}

template<typename T> FORCEINLINE void LinkedList<T>::DeleteNext(LinkedListNode<T> *i_pos)
{
	ASSERT(i_pos != nullptr);
	if (i_pos->Next == nullptr) return;

	LinkedListNode<T> *temp = i_pos->Next->Next;
	delete i_pos->Next;
	i_pos->Next = temp;
}




template<typename T> FORCEINLINE List<T>::List()
	: head_(nullptr), tail_(nullptr) 
{
}

template<typename T> FORCEINLINE void List<T>::PushHead(const T &i_val)
{
	ListNode<T> *node = new ListNode<T>(i_val, nullptr, head_);
	if (head_ != nullptr)
	{
		head_->Previous = node;
	}
	head_ = node;
	if (tail_ == nullptr)
	{
		tail_ = node;
	}
}

template<typename T> FORCEINLINE void List<T>::PopHead()
{
	if (head_ == nullptr) return;
	ListNode<T> *temp = head_;
	if (head_->Next)
	{
		head_->Next->Previous = nullptr;
	}
	head_ = head_->Next;
	
	if (tail_ == temp)
	{
		tail_ = nullptr;
	}
	delete temp;
}

template<typename T> FORCEINLINE void List<T>::PushTail(const T &i_val)
{
	ListNode<T> *node = new ListNode<T>(i_val, tail_, nullptr);
	if (tail_ != nullptr)
	{
		tail_->Next = node;
	}
	tail_ = node;
	if (head_ == nullptr)
	{
		head_ = node;
	}
}
template<typename T> FORCEINLINE void List<T>::PopTail()
{
	if (tail_ == nullptr) return;
	ListNode<T> *temp = tail_;
	if (tail_->Previous)
	{
		tail_->Previous->Next = nullptr;
	}
	tail_ = tail_->Previous;

	if (head_ == temp)
	{
		head_ = nullptr;
	}
	delete temp;
}

template<typename T> FORCEINLINE void List<T>::InsertNext(ListNode<T> *i_pos, const T &i_val)
{
	ASSERT(i_pos != nullptr);
	ListNode<T> *temp = i_pos->Next;
	ListNode<T> *node = new ListNode<T>(i_val, i_pos, temp);
	i_pos->Next = node;
	if (temp != nullptr)
	{
		temp->Previous = node;
	}
	else
	{
		tail_ = node;
	}
}

template<typename T> FORCEINLINE void List<T>::Delete(ListNode<T> *i_pos)
{
	ASSERT(i_pos != nullptr);
	if (i_pos->Previous != nullptr)
	{
		i_pos->Previous->Next = i_pos->Next;
	}
	else
	{
		head_ = i_pos->Next;
	}
	if (i_pos->Next != nullptr)
	{
		i_pos->Next->Previous = i_pos->Previous;
	}
	else
	{
		tail_ = i_pos->Previous;
	}
	delete i_pos;
}

