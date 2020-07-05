#ifndef A3_AVL_TREE_HPP
#define A3_AVL_TREE_HPP

#include "a3_binary_search_tree.hpp"


class AVLTree: public BinarySearchTree
{
private:
	
	Node* rotateRight(Node* & n);
	Node* leftRight(Node* & n);
	Node* rightLeft(Node* & n);
	int height1(Node* & temp);
	int diff(Node* & n);
	void balance(Node* & n);
	public:
	// Overriden insert and remove functions 
	// Do not modify these definitions
    Node* rotateLeft(BinarySearchTree::Node* & n);
	bool insert(DataType val);
    bool remove(DataType val);

	// Define additional functions and attributes that you need below
};
#endif

