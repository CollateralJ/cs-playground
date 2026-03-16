#ifndef LL_H
#define LL_H

template <typename T>
class LL 
{
    struct Node 
    {
        T data;
        Node* prev;
        Node* next;
    };
public:
    class Iterator 
    {
    public:
        friend class LL;
        Iterator() : current(nullptr) {}
        Iterator(Node * x) : current(x) {}
        T operator*() const { return current->data; }

        Iterator operator++(int) 
        {
            Iterator i(*this);
            current = current->next;
            return i;
        }
        Iterator operator++()
        {
            current = current->next;
            return *this;   
        }
        Iterator operator--(int)
        {
            Iterator i(*this);
            current = current->prev;
            return i;
        }
        Iterator operator--()
        {
            current = current->prev;
            return *this;
        }

        bool operator==(const Iterator& i) const
        {
            return this->current == i.current;
        }
        bool operator!=(const Iterator& i) const
        {
            return this->current != i.current;
        }

    private:
        Node* current;
    };

    LL();
    LL(const LL<T>&);
    const LL<T>& operator=(const LL<T>&);
    ~LL();
    void push_front(const T&);
    void push_back(const T&);
    Iterator begin() const;
    Iterator end() const;
    void swapNodes(Iterator&, Iterator&);

private:
    Node* head;
    Node* tail;
};

#include "LL.cpp"

#endif