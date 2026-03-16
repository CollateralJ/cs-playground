/*
    Description: Creates a linked list from a file and sorts it via cocktail shaker method
    Input: File of random numbers
    Output: File of numbers sorted from least to greatest
*/
#include <cstdlib>
#include <iostream>
#include "LL.h"
using namespace std;

template <typename T>
LL<T>::LL()
{
   head = nullptr;
   tail = nullptr;
}
//copy constructor
template <typename T>
LL<T>::LL(const LL<T>& copy)
{
    head = nullptr;
    tail = nullptr;
    *this = copy;
}

// deep copy assignment operator
template <typename T>
const LL<T>& LL<T>::operator=(const LL<T>& rhs) 
{
    if (this != &rhs)
    {
        // clear the list
        this->~LL();
        Iterator it;
        it = rhs.begin();
        while(it.current != nullptr){
            push_back(*it); // push the current data from the rhs to this
            ++it; // go next node
            // loop continues if we didnt go to a nullptr
        }
    }
    return *this;
}

//destructor
template <typename T>
LL<T>::~LL() 
{
    Iterator it;
    it = this->begin();
    while(it.current != nullptr){
        Node * t = it.current; // save the current
        ++it; // go next
        delete t; // delete the saved one
        // loop continues if we didnt go to a nullptr
    }
    head = nullptr;
    tail = nullptr;
}

//head insert
template <typename T>
void LL<T>::push_front(const T& item) 
{
    Node * t;
    t = new Node;
    t->next = nullptr; // preset ptrs to nullptr
    t->prev = nullptr;
    t->data = item;

    if(head == nullptr){ // empty list
        head = t;
        tail = t; // new node is front and back
    }
    else{ // list has 1 or more
        t->next = head; // new node points forwards
        head->prev = t; // old head points backwards
        head = t; // update head to new node
    }
}

//tail insert
template <typename T>
void LL<T>::push_back(const T& item) 
{
    Node * t;
    t = new Node;
    t->next = nullptr; // preset ptrs to nullptr
    t->prev = nullptr;
    t->data = item;

    if(tail == nullptr){ // empty list
        head = t;
        tail = t; // new node is front and back
    }
    else{ // list has 1 or more
        t->prev = tail; // new node points backwards
        tail->next = t; // old tail points forwards
        tail = t; // update tail to new node
    }
}

template <typename T>
typename LL<T>::Iterator LL<T>::begin() const
{
    return Iterator(head); // returns an iterator at the start of list
}

template <typename T>
typename LL<T>::Iterator LL<T>::end() const
{
    return Iterator(tail); // returns an iterator at the end of list
}

template <typename T>
void LL<T>::swapNodes(Iterator& it1, Iterator& it2) // it1 is left and it2 is right, ALWAYS
{
    // tell it1's prev's next to point to it2's current node UNLESS it1's prev node doesn't exist
    if (it1.current != head){
        it1.current->prev->next = it2.current;}
    // tell it2's next's prev to point to it1's current node UNLESS it2's next node doesn't exist
    if (it2.current != tail){
        it2.current->next->prev = it1.current;}
    // tell it1's current node that its next node is it2's next node
    // if it2's next node is nullptr then it should be handled correctly anyways
    it1.current->next = it2.current->next;
    // tell it2's current node that its prev node is it1's prev node
    // if it1's prev node is nullptr then it should be handled correctly anyways
    it2.current->prev = it1.current->prev;
    // tell it1's current node that its prev node is it2's current node
    it1.current->prev = it2.current;
    // tell it2's current node that its next node is it1's current node
    it2.current->next = it1.current;
    // make sure it2's current isnt tail after swap (it might have started at tail but should never end as such)
    if (it2.current == tail){
        tail = it1.current;}
    // make sure it1's current isnt head after swap (it might have started as head but should never end as such)
    if (it1.current == head){
        head = it2.current;}
    // increment it2 so it points to the same index of the list as when the function started
    --it1;
    // increment it2 so it points to the same index of the list as when the function started
    ++it2;
}
