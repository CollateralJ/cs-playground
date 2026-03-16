#include <cstdlib>

#ifndef LL_H
#define LL_H

template <class Type>
class LL
{
private:
	struct node
	{
		Type data;
		node * next;
		node * prev;
	};

	node * head;
	node * tail;
	std::size_t size;

public:
	class Iterator
	{
	private:
		node * current;
	public:
		friend class LL;
		Iterator() : current(nullptr) {}
		Iterator(node * p) : current(p) {}
		Type operator*() { return current->data; }

		Iterator operator++() 
		{
			current = current->next;
			return *this;
		}
		Iterator operator++(int)
		{
			Iterator i(*this);
			current = current->next;
			return i;
		}

		Iterator operator--() 
		{
			current = current->prev;
			return *this;
		}
		Iterator operator--(int)
		{
			Iterator i(*this);
			current = current->prev;
			return i;
		}

		bool operator==(const Iterator& rhs) 
		{ 
			return this->current == rhs.current;
		}

		bool operator!=(const Iterator& rhs) 
		{ 
			return this->current != rhs.current;
		}
	};

	LL() : head(nullptr), tail(nullptr), size(0) {}
	LL(const LL<Type>&);
	const LL<Type>& operator=(const LL<Type>&);
	~LL();
	Iterator begin() { return Iterator(head); }
	Iterator end() { return Iterator(tail); }
	std::size_t getSize() const { return size;}
	void push_back(const Type&);
	bool isEmpty() const { return head == nullptr; }

	void push_back(Iterator&);
	Iterator pop_front();

	Iterator spliceOut(Iterator&);
	void insertion(Iterator&, Iterator&);
	
};

#include "LL.cpp"

#endif