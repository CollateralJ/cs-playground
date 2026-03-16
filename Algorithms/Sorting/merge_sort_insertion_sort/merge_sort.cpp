/*
    Description: Creates a linked list from a file and sorts it via merge sort
    Input: File of random numbers
    Output: File of numbers sorted from least to greatest
*/
#include "LL.h"
#include <iostream>
using namespace std;
LL <int> mergeSort (LL<int>&, LL<int>::Iterator, LL<int>::Iterator);
LL<int> merge (LL<int>&, LL<int>&);
void output(LL<int>);

int count = 0;

int main()
{
    LL<int> list;
	int x; // build list
    while(cin >> x){
        list.push_back(x);
    }
    LL<int>::Iterator front = list.begin();
    LL<int>::Iterator back = list.end();
    LL<int> sortedList = mergeSort(list, front, back);
    output(sortedList);
    cout << "count: " << count << endl;
    return 0;
}

// merge sort function. breaks the lists down until they are 1 size (sorted) arrays
// then merges them back together in the right order
LL<int> mergeSort (LL<int>& list, LL<int>::Iterator front, LL<int>::Iterator back){
    // check if the left has passed the right
    if (front == back){ // size 1 or 0 list
        return list;
    }
    // find the middle point of list
    LL<int>::Iterator mid;
    mid = list.begin();
    for(int i = 0; i < (list.getSize())/2; i++){
        mid++;
    }
    // create two lists that will be merged after
    LL<int> leftList;
    LL<int> rightList;
    LL<int>::Iterator t, node; // go through the list to create left and right
    for(t = list.begin(); t != mid; t = list.begin()){ // form left list
        node = list.pop_front();
        leftList.push_back(node);
    }
    for(; t != nullptr; t = list.begin()){ // form right list
        node = list.pop_front();
        rightList.push_back(node);
    }
    // pass in the new lists with their begin and ends
    leftList = mergeSort(leftList, leftList.begin(), leftList.end());
    rightList = mergeSort(rightList, rightList.begin(), rightList.end());
    LL<int> mergedList = merge(leftList, rightList);
    return mergedList; // merge and return
}

LL<int> merge (LL<int>& leftList, LL<int>& rightList){
    LL<int> combinedList;
    LL<int>::Iterator itL, itR; // left list and right list iterators
    // pop from both lists
    itL = leftList.pop_front();
    itR = rightList.pop_front();
    // compare the two
    while((itL != nullptr) && (itR != nullptr)){ // keep going until one reaches end
        count++;
        if (*itL <= *itR){ // push back the smaller of the two into new list
            combinedList.push_back(itL); // add the smaller node
            itL = leftList.pop_front(); // pop new node for next iteration
        }
        else{
            combinedList.push_back(itR); // add the smaller node
            itR = rightList.pop_front(); // pop new node for next iteration
        }
    }
    // after the last loop breaks one of the lists can still contain items
    // lets just make sure we clear those out (only one of the following loops will run)
    while(itL != nullptr){
        combinedList.push_back(itL);
        itL = leftList.pop_front(); // pop new node for next iteration
    }
    while(itR != nullptr){
        combinedList.push_back(itR);
        itR = rightList.pop_front(); // pop new node for next iteration
    }
    return combinedList;
}

void output(LL<int> list){
    if (list.begin() == nullptr){
        cout << "(debug) list is empty!\n";
    }
    for(LL<int>::Iterator i = list.begin(); i != nullptr; i++){
        cout << *i << endl;
    }
}