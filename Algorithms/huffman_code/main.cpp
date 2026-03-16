/*
    Description: takes letters and frequencies from file and decodes another file with them using huffman prefix codes
    Input: file of letters and frequencies, plus optional file to decode
    Output: either the letters and their frequencies, or that and a decoded file as well
*/

// #include "binTree.h"
#include "binTree.cpp"
#include <iostream>
#include <list>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

int main(int args, char * argv[])
{
    char chars[30]; // store the chars for later loops
	// use a LL or vector to make a "forest" of bintrees
    // these will be read from file one at a time
    ifstream letters(argv[1]); // open first file
    int c, f; // character and frequency read in variables
    vector<binTree*> forest; // vector to hold our forest
    for(int i = 0; letters >> c; i++){ // loop to construct the forest
        letters >> f;
        chars[i] = c;
        binTree* tree = new binTree(static_cast<char>(c), f); // make the new node pointer
        forest.push_back(tree); // add to forest
    } 
    letters.close(); // forest should now contain all of the objects


    // to construct the tree take the two nodes with the smallest frequency and store their pointers
    // removing them from the list
    // and combine them with the constructor
    // do this until forest has only one tree left, then our tree is complete
    while(forest.size() > 1){
        // find the two nodes with the smallest frequency
        int lowest = forest[0]->getFrequency();
        int index = 0;
        binTree* least = forest[0];
        // get the very smallest
        for(int i = 0; i < forest.size(); i++){
            if (forest[i]->getFrequency() < lowest){
                lowest = forest[i]->getFrequency();
                least = forest[i];
                index = i;
            }
        }
        forest.erase(forest.begin() + index); // remove from the list

        binTree* second = forest[0]; 
        lowest = forest[0]->getFrequency();
        index = 0;
        // get the second smallest
        for(int i = 0; i < forest.size(); i++){
            if (forest[i]->getFrequency() < lowest){
                lowest = forest[i]->getFrequency();
                second = forest[i];
                index = i;
            }
        }
        forest.erase(forest.begin() + index); // remove from the list
        binTree* tree = new binTree(least, second);
        forest.push_back(tree);
        
    }
    string dict[30][2] = {};
    // we need to contruct a 2d array (basically a dictionary of characters and their prefix code) for later
    for(int i = 0; i < (sizeof(chars)/sizeof(chars[0])); i++){
        dict[i][0] = chars[i];
        dict[i][1] = forest[0]->getPrefixCode(chars[i]);
        if (args == 2){
            switch (chars[i]){
                case ' ':
                    dict[i][0] = "SPACE";
                    break;
                case '\n':
                    dict[i][0] = "RETURN";
                    break;
            }
            cout << dict[i][0] << " maps to " << dict[i][1] << endl;
        }
    }
    if (args == 2)
        return 0;

    // if 2 args - just output the characters and prefixes


    // else go through file and use the prefixes to decode
    char bit;
    ifstream bits(argv[2]);
    string code = "";
    while (bits.get(bit)) { // read a 0 or 1
        code += bit; // build string
        for(int i = 0; i < (sizeof(chars)/sizeof(chars[0])); i++){ // search the dictionary for the character
            if (dict[i][1] == code){
                cout << dict[i][0];
                code = "";
                break;
            }
        }
    }
    delete forest[0];
    return 0;
}

