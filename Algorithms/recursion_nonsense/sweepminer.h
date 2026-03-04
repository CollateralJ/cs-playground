#ifndef MINESWEEPER_H
#define MINESWEEPER_H

#include <string>
#include <iostream>

using namespace std;

//Struct for a 2D position with an x and y value
struct Position
{
	int x, y;
	bool operator == (Position& other) { return x == other.x && y == other.y; }
	Position addPosition(int delta_x, int delta_y) { return Position(x + delta_x, y + delta_y); }

	Position(int x, int y) { this->x = x; this->y = y; }
};

//Class representing a Minesweeper-esque game to sweep for bombs. 
class Sweepminer
{
	const string flag = "\033[31mF\033[0m"; //String for printing the red F for a bomb

	int** board;
	int width, height;
	void allocateBoard();
	void deallocateBoard();
	int getDistanceAtPosition(Position pos) const { return board[pos.y][pos.x]; }
	void setDistanceAtPosition(Position pos, int dist) { board[pos.y][pos.x] = dist; }
public:
	void populateBoard(const int& bomb_count);
	void generateDistances(Position pos, int distance);
	void printBoard();

	Sweepminer(int width, int height, int num_bombs = 3, int seed = 1024) 
	{
		this->width = width;
		this->height = height;
		allocateBoard();

		//Fill the board with bombs
		srand(seed);
		populateBoard(num_bombs);
	}

	~Sweepminer()
	{
		deallocateBoard();
	}
};

#endif
