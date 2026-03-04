/* 
Description: matryoshka doll recursion experiments
Input: none, hard coded except for option select
Output: which doll is present and the ones inside
*/


#include "matryoshka.h"

/*This should print the doll that is being opened, and then open the next doll */
void MatryoshkaDoll::openDoll()
{
	//TODO: Finish the function
	//
	//To do the same if there is a doll inside this one.
	printDoll();
	if(dollInside != nullptr)
	{
		dollInside->openDoll();
	}
}

/* Deallocate any dolls contained within this one if they exist*/
MatryoshkaDoll::~MatryoshkaDoll()
{
	//TODO: Finish the function
	//
	delete dollInside;
}

/* Prints that the doll is being opened alongside its id */
void MatryoshkaDoll::printDoll()
{
	cout << "Opening doll (" << id << " / " << count << ")" << endl;
}