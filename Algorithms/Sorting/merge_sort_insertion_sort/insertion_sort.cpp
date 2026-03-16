/*
    Description: Creates a linked list from a file and sorts it via insertion sort
    Input: File of random numbers
    Output: File of numbers sorted from least to greatest
*/
#include "LL.h"
#include <iostream>
using namespace std;
void output(LL<int>);
int count = 0;

int main()
{
    // build list
    LL<int> list;
	int x;

    while(cin >> x){
        list.push_back(x);
    }
    // start the key at the beginning of list
    LL<int>::Iterator i, j, key;
    // outer loop
    i = list.begin();
    for (i++; i != nullptr; i++){
        // key = i; // key = arr[i]
        key = i;
        // inner loop
        for (j = i; (j != nullptr); j--){
            if (j == i){
                j--; // for setting j = i - 1 first time
            }
            count++;
            if (*j <= *key){
                break; // leave loop when arr[j] <= key
                
            }
        }
        key = list.spliceOut(key); // remove the key
        list.insertion(j, key); // place the key
    }
    output(list); // output sorted list
    cout << "count: " << count << endl;
    return 0;
}

void output(LL<int> list){
    for(LL<int>::Iterator i = list.begin(); i != nullptr; i++){
        cout << *i << endl;
    }
}