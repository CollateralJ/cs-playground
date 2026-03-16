/*
    Description: function definitions for use with huffman prefix codes
    Input: n/a
    Output: n/a
*/


#include "binTree.h"
#include <iostream>
using namespace std;

binTree::binTree()
{
    root = new binTreeNode;
}


binTree::binTree(char letter, int frequency)
{
    // add a single node as the root
    root = new binTreeNode;
    root->letter = letter;
    root->frequency = frequency;
    root->left = nullptr;
    root->right = nullptr;
}

binTree::binTree(binTree * t1, binTree * t2)
{
    // combine two trees into 1
    root = new binTreeNode;
    root->letter = '\0'; // mark this node as null
    root->left = t1->root; // add the children
    root->right = t2->root; 
    root->frequency = t1->root->frequency + t2->root->frequency; // combine the frequencies
}


binTree::~binTree() // call destroy tree on the root of this tree
{
    destroyTree(root);
}


void binTree::destroyTree(binTreeNode * r)
{
    // deallocate this in a post order type traversal   
    // use the postorder from class but instead of cout use delete
    if (r == nullptr)
        return;
    destroyTree(r->left);
    destroyTree(r->right);
    delete r;
    
    return;
}

int binTree::getFrequency() const // return the root node's frequency
{
    return root->frequency;
}


std::string binTree::getPrefixCode(char letter)
{
    return getPrefixCode(root, letter); // call the private func
}

std::string binTree::getPrefixCode(binTreeNode * r, char letter)
{
    if (r == nullptr) // -1 height
        return "\0";
    string leftCode = getPrefixCode(r->left, letter); // check the left side
    if (leftCode != "\0"){ // if != then we are in the right branch
        if (leftCode == " ") // check if its coming from the leaf
            leftCode = "0";
        else
            leftCode = "0" + leftCode;
        return leftCode;
    }
    string rightCode = getPrefixCode(r->right, letter); // check the right side
    if (rightCode != "\0"){ // if != then we are in the right branch
        if (rightCode == " ") // check if its coming from the leaf
            rightCode = "1";
        else
            rightCode = "1" + rightCode;
        return rightCode;
    }
    if (letter == r->letter) // match found on a leaf!
        return " "; // blank string if char found
    return "\0"; // null string if char not found
	// recursively find the letter and return up the prefix code
}

