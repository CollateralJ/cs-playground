/*
    Description: function definitions for use with sorting algorithms
    Input: n/a
    Output: n/a
*/
#include <cstdlib>
#include <iostream>
#include "LL.h"
using namespace std;

template <class Type>
LL<Type>::~LL()
{
    Iterator it;
    it = this->begin();
    while(it.current != nullptr){
        node * t = it.current; // save the current
        ++it; // go next
        delete t; // delete the saved one
        // loop continues if we didnt go to a nullptr
    }
    head = nullptr;
    tail = nullptr;
	size = 0;
}

template <class Type>
LL<Type>::LL(const LL<Type>& copy)
{
	// do the usual constructor stuff, then attempt to copy
    head = nullptr;
    tail = nullptr;
	size = 0;
    *this = copy;
}

// deep copy assignment operator
template <class Type>
const LL<Type>& LL<Type>::operator=(const LL<Type>& rhs)
{
    if (this != &rhs)
    {
        this->~LL(); // clear the list
        Iterator it;
        it = const_cast<LL<Type>&>(rhs).begin(); // compiler issue and i cant change ll.h
        while(it.current != nullptr){
            push_back(*it); // push the current data from the rhs to this
            ++it; // go next node
            // loop continues if we didnt go to a nullptr
        }
		// cout << "(debug) copying an LL of size: " << rhs.size << endl;
		size = rhs.size;
    }
    return *this;
}

template <class Type>
void LL<Type>::push_back(const Type& t)
{
    node * tmp; // make new node + pointer of correct type
    tmp = new node;
    tmp->next = nullptr; // preset ptrs to nullptr
    tmp->prev = nullptr;
    tmp->data = t; // assign the data

    if(tail == nullptr){ // empty list
        head = tmp;
        tail = tmp; // new node is front and back
		size = 1; // add to the size of the list
    }
    else{ // list has 1 or more
        tmp->prev = tail; // new node points backwards
        tail->next = tmp; // old tail points forwards
        tail = tmp; // update tail to new node
		size += 1; // add to the size of the list
    }
	
}

template <class Type> // push back except we already have the node
void LL<Type>::push_back(LL<Type>::Iterator& // node is pointed to by iterator
	nodeToBePushedBack) // i hate this var name lol
{
	if (nodeToBePushedBack == nullptr){
		return;
	}
    nodeToBePushedBack.current->next = nullptr; // preset ptrs to nullptr
    nodeToBePushedBack.current->prev = nullptr;
	if(tail == nullptr){ // empty list
        head = nodeToBePushedBack.current;
        tail = nodeToBePushedBack.current; // new node is front and back
		size = 1; // add to the size of the list
    }
    else{ // list has 1 or more
        nodeToBePushedBack.current->prev = tail; // new node points backwards
        tail->next = nodeToBePushedBack.current; // old tail points forwards
        tail = nodeToBePushedBack.current; // update tail to new node
		size += 1; // add to the size of the list
    }
}

template <class Type> // pop a node from the front AND return it
typename LL<Type>::Iterator LL<Type>::pop_front()
{
	Iterator it = begin();
	// check list is empty (0 size)
	if (isEmpty()){
		return it; // return an empty iterator
	}
	it.current = head; // make a new iterator as head

	// check if head and tail point to same node (1 size)
	if (head == tail){ // take one node out and null the head/tail
		it.current = head; // point it to head
		head->prev = nullptr; // clear out all of the data
		head->next = nullptr;
		tail->prev = nullptr;
		tail->next = nullptr;
		head = nullptr;
		tail = nullptr;
		size = 0;
		return it; // give back the one node
	}
	// for 2+ size, move the head forward and null its last node
	head = head->next;
	head->prev = nullptr;
	// check if we are making head point to same node as tail (2 size)
	if (head == tail){ // they point to the same node
		// in size 1 LL we typically have all pointers nulled except head, tail on 1 node
		head->next = nullptr;
		tail->prev = nullptr;
		tail->next = nullptr;
	}
	// continue and remove it.currents ties to the old list
	it.current->next = nullptr;
	it.current->prev = nullptr;
	size -= 1;
	return it; // return the it with basically just the current node's data retained
}

template <class Type> // remove a node and return it as an iterator
typename LL<Type>::Iterator LL<Type>::spliceOut(LL<Type>::Iterator& it)
{
	
	// LL<Type>::Iterator it3;
	// // return it3;
	// if (it == nullptr){
	// 	return it3;
	// }
	// save the current node by pointing a new iterator to it
	Iterator it2(it.current);
	if (it == nullptr){
		return it2;
	}


	// point it to the next one
	if (it.current->next == nullptr){
		// cout << "(debug) tried to splice with a missing node in front, moving iterator backwards\n";
		if (it.current->prev != nullptr){
			it.current->prev->next = nullptr;
			tail = it.current->prev;
			it--;
		}
		else{
			// cout << "(debug) couldn't move backwards either, iterator may be undefined\n";
		}
	}
	else if(it.current->prev == nullptr){
		// cout << "(debug) tried to splice with a missing node behind, continuing with modified case\n";
		// we know it.current->next != nullptr since we are in elseif
		it.current->next->prev = nullptr;
		head = it.current->next;
		++it;
	}
	else{
		it.current->prev->next = it.current->next; // update the nodes around it
		it.current->next->prev = it.current->prev;
		++it; // move it to the next index
	}
	// continue and remove it2.currents ties to the old list
	it2.current->next = nullptr;
	it2.current->prev = nullptr;
	size -= 1;

	// return the iterator that points to the node that got removed
	return it2; // basically just the data field
}

template <class Type>
void LL<Type>::insertion(LL<Type>::Iterator& locationOfLL, // put a node in front of first iter
	LL<Type>::Iterator& nodeToBeInserted) // the node we are putting there is the second
{
	if (locationOfLL.current == nullptr){
		// cout << "(debug) insertion modified do to locationOfLL being nil\n";
		// inserting at head
		head->prev = nodeToBeInserted.current; // point old head back
		nodeToBeInserted.current->next = head; // point new head forward
		head = nodeToBeInserted.current; // update head pointer
	}
	else{
		nodeToBeInserted.current->next = locationOfLL.current->next; // first point inserted node forward
	}
	
	if (nodeToBeInserted.current->next != nullptr){ // now look forward and point back if there is a node there
		nodeToBeInserted.current->next->prev = nodeToBeInserted.current; 
	}
	if (locationOfLL.current != nullptr){
		// now look backwards
		nodeToBeInserted.current->prev = locationOfLL.current;
		// finally correctly hook up the prev to this node
		locationOfLL.current->next = nodeToBeInserted.current;
	}
	else{
		nodeToBeInserted.current->prev = nullptr;
	}

	size += 1;
}
