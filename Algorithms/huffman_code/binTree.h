#include <string>


class binTree
{	
private:
	struct binTreeNode
  	{
    	char letter;
    	int frequency;
    	binTreeNode * left, * right;
  	};

	std::string getPrefixCode(binTreeNode*, char);
	void destroyTree(binTreeNode*);
	binTreeNode * root;

public:
  	binTree();
  	binTree(char, int);
  	binTree(binTree*, binTree*);
  	~binTree();
  	int getFrequency() const;
  	std::string getPrefixCode(char);
	void preorder();
	void preorder(binTreeNode * r);
};