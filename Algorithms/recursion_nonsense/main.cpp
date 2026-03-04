#include "general_recursion.h"
#include "sweepminer.h"
#include "matryoshka.h"
#include <time.h>
#include <cstdlib>
#include <iostream>

using namespace std;

//Static initializers
int MatryoshkaDoll::count = 0;
const int DOLL_AMOUNT = 9; //How many dolls to make
const int BOMB_COUNT = 5;
const int MINESWEEPER_WIDTH = 16;
const int MINESWEEPER_HEIGHT = 8;

//Function prototypes
int readInteger(istream& in);

//Variables for test cases
const int SEARCH_ARR_SIZE = 16;
int arr[SEARCH_ARR_SIZE]{ 1, 2, 6, 8, 10, 12, 13, 18,
			19, 20, 21, 21, 22, 23, 28, 30 }; //Array for the search function

int main()
{
	//Print a message about what functions are available to test
	cout << "Please select a function to test:" << endl;
	cout << "(0) countBackwards" << endl;
	cout << "(1) sumToN" << endl;
	cout << "(2) isValueInArray" << endl;
	cout << "(3) Matryoshka class" << endl;
	cout << "(4) Sweepminer class" << endl;

	//Test cases for the functions
	switch (readInteger(cin))
	{
		case 0: //Testing countBackwards
			cout << "Counting backwards from 4:" << endl;
			countBackwards(4);
			cout << endl << "Counting backwards from 86:" << endl;
			countBackwards(86);
			break;	
		case 1: //Testing sumToN
			cout << "Sum of 1 to 10: " << sumToN(10) << endl;
			cout << "Sum of 1 to 99: " << sumToN(99) << endl;
			break;	
		case 2: //Testing isValueInArray
			cout << "Array:" << endl;
			for (int i = 0; i < SEARCH_ARR_SIZE - 1; i++)
				cout << arr[i] << ", ";
			cout << arr[SEARCH_ARR_SIZE - 1] << endl;

			cout << "Is 18 in the array? ";
			cout << (isValueInArray(arr, 0, SEARCH_ARR_SIZE, 18) ? "Yes" : "No") << endl;

			cout << "Is 9 in the array? ";
			cout << (isValueInArray(arr, 0, SEARCH_ARR_SIZE, 9) ? "Yes" : "No") << endl;

			cout << "Is 23 in the array? ";
			cout << (isValueInArray(arr, 0, SEARCH_ARR_SIZE, 23) ? "Yes" : "No") << endl;
			break;	
		case 3: //Testing the Matryoshka class
			MatryoshkaDoll* doll;
			doll = new MatryoshkaDoll(nullptr); //Make the center doll
			for (int i = 0; i < DOLL_AMOUNT - 1; i++)
				doll = new MatryoshkaDoll(doll); //Make a new doll with the last one in the center

			doll->openDoll();
			delete doll;
			break;
		case 4: //Testing the Minesweeper class
			Sweepminer sweepminer =  //Make the board
				Sweepminer(MINESWEEPER_WIDTH, MINESWEEPER_HEIGHT, BOMB_COUNT); 
			sweepminer.printBoard(); //Print the result for testing
			break;
	}

	return 0;
}


//Helper function to read a positive integer from a stream with error checking
int readInteger(istream& in)
{
	int value;
	cin >> value;
	while (cin.fail() || value < 0)
	{
		cout << "Error, please enter a non-negative integer" << endl;
		in.ignore(1024);
		in.clear();
		in >> value;
	}

	return value;
}
