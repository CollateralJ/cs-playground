/*
    Description: Creates a linked list from a file and sorts it via cocktail shaker method
    Input: File of random numbers
    Output: File of numbers sorted from least to greatest
*/
#include "LL.h"
#include <iostream>
using namespace std;

void output(LL<int>);

int main()
{
LL<int> list;
LL<int>::Iterator lWall, rWall, l, r;
int x;
while(cin >> x){
    list.push_back(x);
}

// edge case needed of small arr for the following statements
l = list.begin();
r = list.begin();
r++;

// sorting
while(true){
    // traverse forwards
    while(r != rWall){

        if (*l > *r){ // if the left is greater than the right, swap
            list.swapNodes(l, r);
        }

        // move both to the right
        l++;
        r++;
    }
    if (r == rWall){ // edge case for when r gets set to null on first iteration
            r = l;
            l--;
        }
    // decrease the end one node
    if (rWall != nullptr){ // handle the edge case of first iteration
        rWall--;
    } else {
        rWall = list.end();
    }

    // check if the start and end walls have reached eachother
    if (lWall == rWall){
        break;
    }

    // traverse backwards
    while(l != lWall){
        if (*l > *r){ // if the left is greater than the right, swap
            list.swapNodes(l, r);
        }
        // move both to the left
        l--;
        r--;
    }
    if (l == lWall){ // edge case for when we go past the wall
            l = r;
            r++;
        }
    // increase the start one node
    if (lWall != nullptr){ // handle the edge case of first iteration
        lWall++;
    } else {
        lWall = list.begin();
    }

    // check if the start and end walls have reached eachother
    if (lWall == rWall){
        break;
    }
}
// output to file
output(list);


return 0;
}

void output(LL<int> list){
    for(LL<int>::Iterator i = list.begin(); i != nullptr; i++){
        cout << *i << endl;
    }
}
