/* 
Description: various recursion functions
Input: none, hard coded except for option select
Output: various, backwards count, sum of n numbers, isvalue in array
*/

#ifndef RECURSION_H
#define RECURSION_H

#include <iostream>
#include <string>

using namespace std;

//This file contains several functions that solve generic recursive problems. 
//TODO: Please finish each and test them using the provided main

/* This function should recursively print all integers backwards stopping at 0 */
void countBackwards(int num)
{
	// base case
	if(num <= 0){
		cout << num << endl;
		return;
	}
	// function
	cout << num << ", ";
	// reduction and recursive call
	countBackwards(num-1);
}

/* Recursively compute the sum up to n of a number. */
int sumToN(int n)
{
	//
	//To problem solve, consider how you might break up the problem to reduce it
	//And what a trivial base case that is easy to sum might be
    if(n==0) //Base condition
        return 0;
    return (n+sumToN(n-1));
}

/* search array checking if value is in it */
bool isValueInArray(int arr[], int start, int length, const int& value)
{
	// base case
	// if(start == length-1){return false;} i think base case is == 0
	// if(start == length && arr[start] == value){return true;}
	/*
	*Check if the array is of length 0, in which case, the solution is trivial
	*(we can't find any value in an empty array)
	*/
	if(length == 0){return false;}

	/*
	*Check if the current element being looked at is a match
	*If so, we found it
	*/
	if(arr[start] == value){return true;}
	/*
	*If we did not find the element, recursively search for it.
	*Knowing that the list is already sorted, if the value is less than the middle of the list,
	*search the sub array to the left, else search the array to the right.
	*Please see the provided video for a more detalied explanation */
	
	int leftLength = length / 2; //Length of the left array
	int rightLength = length - leftLength; //Length of the right subarray
	int middle = start + leftLength;
    if(value == arr[middle]){return true;}

	return (value < arr[middle]) ? (isValueInArray(arr, start, leftLength, value)) : (isValueInArray(arr, (start + leftLength + 1), rightLength, value));
}

#endif
