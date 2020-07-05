//Shilpan Shah (206003389) and Nikhil Keswani (20611258)
#include<iostream>
#include<cmath> 
#include "a3_binary_search_tree.hpp"
using namespace std;


typedef BinarySearchTree::DataType DataType;

 
BinarySearchTree::BinarySearchTree()
{
	root_=NULL;
	size_=0;
}
BinarySearchTree::Node::Node(DataType newval)
{
	val=newval;
	left=NULL;
	right=NULL;
	
}
 BinarySearchTree::DataType BinarySearchTree:: max() const
{
	
	Node* temp = root_;
	
	while(temp->right)
	{
		temp = temp->right;
	}
	return temp->val;
}
int BinarySearchTree:: getNodeDepth (Node* n) const
{
	if(n == NULL) return -1;
	
		return 1+ fmax(getNodeDepth(n->left) , getNodeDepth(n->right));
//	int rightHeight = getNodeDepth(n->right);	
//	if(leftHeight > rightHeight)
//		return leftHeight + 1;
//	else
//		return rightHeight + 1;

	
}
BinarySearchTree::~BinarySearchTree()
{
 	
}
 
unsigned int BinarySearchTree::size() const
{
	return size_;
}



BinarySearchTree::DataType BinarySearchTree::min() const
{
	
	Node* temp= root_;
	
	while(temp->left!=NULL)
	{
		temp= temp->left;
	}
	return temp->val;
}
unsigned int BinarySearchTree::depth() const
{
	
	return getNodeDepth(root_);

}

bool BinarySearchTree::exists(DataType val) const
{
	if( size_==0)
		return false;
	else if(size_==1)
	{
		if(root_->val==val)
			return true;
		return false;
	}

	Node* temp = root_;
	while (temp!= NULL)
	{
		if(temp->val==val)
			return true;
		else if (val < temp->val)
		{
			temp = temp->left;
		}
		else 
			temp = temp->right;		
	}
	return false;
}	

BinarySearchTree::Node* BinarySearchTree:: getRootNode()
{
	return root_; 
}
BinarySearchTree:: Node** BinarySearchTree::getRootNodeAddress()
{
	return &root_;
}
bool BinarySearchTree::insert(DataType val)
{
	if( root_==NULL)
	{
	
		root_= new Node(val);
		size_++;
		return true;
	}
	// if it exists return false
	if(exists(val))
	return false;	
	Node* temp= root_;
	Node* temp1= root_; 
	while(temp!=NULL)
	{
		if(val<temp->val)
		{
			temp1=temp;
			temp=temp->left;
		}
		else
		{
			temp1=temp;
			temp=temp->right;
		}
	}
	//temp is now pointing to NULL and temp1 is pointoiing to where you should insert
		if(val<temp1->val)
		{
			temp1->left= new Node(val);
		}
		else
			temp1->right= new Node(val);
		size_++;
		
	return true; 
}

bool BinarySearchTree::remove(DataType val)
{
	if(!exists(val) || !root_)
		return false;
		
	else	if (size_==1)
	{
		delete root_;
		root_=NULL;
	//	delete temp, parent;
		size_--;
		return true;
			}
	
	Node* temp = root_;
	
	Node* parent=root_;
//	cout<<"root:"<<root_->val<<endl;
	while (temp->val!=val) //temp is the node we want to delete when we exist the loop
	{			
		parent=temp;
	//	cout<<"search: "<<parent->val<<endl;
		if(val<parent->val)
			temp=parent->left;
		else
			temp=parent->right;
			
	}

	if ( temp->right==NULL && temp->left==NULL)// deleting leaf node case
	{
		if (parent->left==temp)
			parent->left=NULL;
		else
			parent->right=NULL;
			
		delete temp; //first case, deleting leaf nodes.. this is a dnalging pointer ..fix it. get to the parent root and then delete the child node. 
		size_--;
		return true;
	}
	
	else if( temp==root_ && temp->left!=NULL && temp->right==NULL)// delete temp where tempp is pointing to root
	{
		root_=root_->left;
		delete temp;
		//delete parent;
		size_--;
		temp=NULL;
	}
	else if( temp==root_ && temp->right!=NULL && temp->left==NULL)//delete temp where tempp is pointing to root
	{
		root_=root_->right;
		delete temp;
		//delete parent;
		size_--;
		temp=NULL;
	}
	else if(parent->left==temp && temp->right==NULL && temp->left!=NULL )// one child case 
	{
		//parent->val = temp->val;
		parent->left= temp->left;
		delete temp;
		size_--;
		return true;
	}
	else if( parent->right==temp && temp->left==NULL && temp->right!=NULL)
	{
	//	parent->val = temp->val;
		parent->right= temp->right;
		delete temp;
		size_--;
		return true;
	}
	else if( parent->left== temp && temp->left==NULL && temp->right!=NULL)
	{
	//	parent->val = temp->val;
		parent->left= temp->right;
		delete temp;
		size_--;
		return true;
	}
	else if( parent->right==temp && temp->left!=NULL && temp->right==NULL)
	{
	//	parent->val = temp->val;
		parent->right= temp->left;
		delete temp;
		size_--;
		return true;
	}
	
	
	
	
	
	
	
	else if(temp->left!=NULL && temp->right!=NULL)//case 3. two child nodes
	{
		Node* successor = temp->right; 
		Node* succParent = temp;
		while (successor->left != NULL) //search for the smallest value in the right subtree, which is the furthest left node in that subtree
		{
			succParent = successor;
			successor = successor->left;
		}
		
		if (succParent == temp) //special case if succParent equals toDelete since this means we have not gone left yet
		{
			succParent->right = successor->right;// bypassing part .. this assumes successor left is null.
		}
		else if (successor->right != NULL) //if the successor has a right child, move it up the tree
		{
			
		succParent->left = successor->right;
		}
		else //otherwise, succParent's left should point to nothing...... successor is leaf node. you want to deletet hte leaf node 
		{
			succParent->left = NULL;
		}
		
		temp->val = successor->val;
		successor->left = NULL;
		successor->right = NULL;
		delete successor;
		successor = NULL;
		
		size_--;
		
		
}
		return true;
		
	}
		
		
			
//		//find min on the right subtree 
//		Node* templow= temp->right;
//		while(templow->left)
//			templow=templow->left;
//		temp->val=templow->val;
//		temp=templow;
//		//delete temp;
//				cout<<"worked upto here";
//	
	//}
//	else if (parent->right == NULL && parent->left != NULL)
//	{
//		temp->val = temp->left->val;
//		delete temp->left;
//		temp->left = NULL;
//		size_--;
//		return true;
//	}
//	else if (temp->left == NULL && temp->right != NULL)
//	{
//		temp->val = temp->right->val;
//		delete temp->right;
//		temp->right = NULL;
//		size_--;
//		return true;
//	}
//	else if(parent->left != NULL && parent->right != NULL )//case 3. if it has two child nodes
//	{
//		Node*temp1= parent->right;
//		while(temp1->left!=NULL)// goes to leaf node  node with min value
//	{
//		temp1= temp1->left;
//	}
//	parent->val= temp1->val;
//	delete temp1;
//
//	size_--;
//	return true;
//	}
	//else
	//	return false;
//	
//	
//void BinarySearchTree::print() const
//{
//	if (root_ == NULL) //if empty, do nothing
//	{
//		return;
//	}
//	
//	std::queue<Node> Q;
//	Q.push(*root_);
//	while (!Q.empty()) //breadth-first traversal until queue is empty
//	{
//		Node N = Q.front();
//		Q.pop();
//		cout << N.val << " ";
//		if (N.left != NULL)
//		{
//			Q.push(*(N.left));		
//		}
//		if (N.right != NULL)
//		{
//			Q.push(*(N.right));
//		}
//	}
//}
void BinarySearchTree::updateNodeBalance(Node* n)//calculates avlbalance for each node
{
	if (n== NULL)
		return; 
	n->avlBalance = getNodeDepth(n->left) - getNodeDepth(n->right);
	updateNodeBalance(n->right);
	updateNodeBalance(n->left);
}

/*
BINARY SEARCH TREE TESTING RESULTS
**********************************
Test1: New tree is valid
TEST PASSED
Test2: Test a tree with one node
TEST PASSED
Test3: Insert, remove, and size on linear list formation with three elements
TEST PASSED
Test4: Test removal of a node with one child
TEST PASSED
Test5: Insert multiple elements and remove till nothing remains
TEST PASSED
Test6: Test removal of root node when both children of root have two children
TEST PASSED
Test7: Test depth with many inserts & some removes
TEST PASSED
Test8: Lots of inserts and removes
TEST PASSED
/*
*/
