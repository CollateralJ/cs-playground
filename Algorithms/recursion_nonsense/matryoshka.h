#ifndef MATRYOSHKA_H
#define MATRYOSHKA_H

#include <iostream>
#include <string>

using namespace std;

//MatryoshkaDoll class for one of the functions you will need to implement
class MatryoshkaDoll {
	int id;
	static int count;
	MatryoshkaDoll* dollInside;
public:
	void openDoll();
	void printDoll();

	MatryoshkaDoll(MatryoshkaDoll* innerDoll) {
		dollInside = innerDoll;
		id = count;
		count++;
	}
	~MatryoshkaDoll();
};

#endif
