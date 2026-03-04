/* 
Description: sweepminer board displayer
Input: none, hard coded except for option select
Output: board of nodes showing how close they are to a bomb
*/

#include "sweepminer.h"
#include <iomanip>
#include <vector> 

using namespace std;

//vv Uncomment next line to get helpful debug statements, but leave commented for final submission vv
//#define DEBUG 

/* Dynamically allocate the 2D board array (please don't linearize) */
void Sweepminer::allocateBoard()
{
	//TODO: 
	//You can assume that the height and width have already been set before calling
	//After allocating, fill the board with maxDist to mark as uninitialized

	int maxDist = width + height + 1; //Maximum possible distance a space could be from bomb
	// allocate the rows
	board = new int * [height];
	for (int i = 0; i < height; i++)
	{
		// allocated the columns
		board[i] = new int [width];
		// initialize all to maxDist
		for(int j = 0; j < width; j++)
		{
			board[i][j] = maxDist;
		}
	}
}

/*Deallocate the 2D board array */
void Sweepminer::deallocateBoard()
{
	for(int i = 0; i < height; i++)
	{
		delete[] board[i];
	}
	delete[] board;
	//TODO: 
	//You can assume that the height and width have already been set before calling
}

/*This function should recursively mark the distances of all squares to the closest bomb. */
void Sweepminer::generateDistances(Position pos, int distance)
{
	//TODO: Finish the function

	//
	if((pos.x > width-1) || (pos.y > height-1) || pos.x < 0 || pos.y < 0){return;} // base case where node is out of bounds
	
	//If the position on the board is already closer to another bomb (i.e.
	//the distance in the current spot is less than the distance param)
	//Then it does not need replaced
	if(distance >= getDistanceAtPosition(pos)){return;} //base case where distance is larger than current mark

	//Otherwise, the distance should be replaced and the adjacent squares should
	//Also have their distances generated as long as they are in bounds
	setDistanceAtPosition(pos, distance); // set new distance to pos
	
	// Position temp = pos; // make temp version of pos
	// temp.addPosition(-1, 0); // swap to adjacent left node
	generateDistances(Position(pos.x-1, pos.y), distance+1);

	// temp = pos; // undo
	// temp.addPosition(0, -1); // swap to adjacent up node
	generateDistances(Position(pos.x, pos.y-1), distance+1);

	// temp = pos; // undo
	// temp.addPosition(1, 0); // swap to adjacent right node
	generateDistances(Position(pos.x+1, pos.y), distance+1);

	// temp = pos; // undo
	// temp.addPosition(0, 1); // swap to adjacent down node
	generateDistances(Position(pos.x, pos.y+1), distance+1);

	//(You can check the bounds as either base cases or before recursively calling)
	//Consider the variables you have access to and how you might check whether a square
	//is within the boundaries of the board (e.g. Something like (-1, 4) is out of
	//bounds of a 10x10 board).

	//Please see the pdf and video for a more detailed explanation

}

//------------------------------------ FUNCTIONS FOR SKELETON ------------------------------------
/* Populates the board by randomly generating where the bombs go and then
calling the generateDistances function to fill in the distances */
void Sweepminer::populateBoard(const int& bomb_count)
{
	vector<Position> bombPositions;
	int i, j;
	Position* temp;
	bool duplicatePosition = false;

	for (i = 0; i < bomb_count; i++)
	{
		//Generate the next bomb position and make sure it's not a duplicate
		temp = new Position(rand() % width, rand() % height);
		while (duplicatePosition)
		{
			duplicatePosition = false;
			for (j = 0; j < (int)bombPositions.size(); j++)
			{
				if (bombPositions[j] == *temp)
				{
					duplicatePosition = true;
					break;
				}
			}
		}

		//Then, add the bomb and generate its distances
		generateDistances(*temp, 0); //Position is temp, distance to bomb is 0
		delete temp;
	
		//Debug print info
		#ifdef DEBUG
		cout << "Board after putting bomb " << i << " at (" << temp.x << ", " << temp.y << ")" << endl;
		printBoard();
		#endif
	}
}

/* Prints all of the distances to flags to the terminal */
void Sweepminer::printBoard()
{
	int i, j;
	cout << setfill('-') << setw(3 * width + 3) << "" << setfill(' ') << endl; //Print top border
	for (i = 0; i < height; i++)
	{
		cout << "| "; //Print left border
		for (j = 0; j < width; j++)
		{
			if (board[i][j] == 0) //If this spot contains a bomb/flag, print its
				cout << " " << flag << " ";
			else //Else print the distance
				cout << setw(2) << right << board[i][j] << " ";
		}
		cout << "|" << endl; //Print right border
	}
	cout << setfill('-') << setw(3 * width + 3) << "" << setfill(' ') << endl; //Print bottom border
}