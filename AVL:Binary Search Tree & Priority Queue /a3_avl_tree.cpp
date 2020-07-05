//Shilpan Shah (206003389) and Nikhil Keswani (20611258)
#include<iostream>
#include<cmath> 
#include "a3_avl_tree.hpp"
#include "a3_binary_search_tree.hpp"


using namespace std;

bool AVLTree::insert(DataType val)
{
	this->BinarySearchTree::insert(val);
	Node* n= this->BinarySearchTree::getRootNode();
	//Node** temp = this->BinarySearchTree::getRootNodeAddress();
	AVLTree::balance(n);//scope operator	
	return true;
}

bool AVLTree::remove(DataType val)
{
	this->BinarySearchTree::remove(val);
	Node* temp= this->BinarySearchTree::getRootNode();
	//update node balance
	//Node** temp = this->BinarySearchTree::getRootNodeAddress();
	balance(temp);
	//scope operator
	return true;
}

//AVLTree::Node* rotateLeft(AVLTree::Node* & parent)
AVLTree::Node* AVLTree::rotateLeft(AVLTree::Node* & n)
{
	AVLTree::Node* temp= n;
			
	temp = n->right;	
	if(temp->right != NULL && temp->left == NULL)
	{
		n->left=temp->right;
		temp->right=NULL;
		int store1= n->val;
		n->val=temp->val;
		temp->val=n->left->val;
  	 	n->left->val=store1;
	}		   
	else if(temp->left != NULL && temp->right == NULL)
	{
		n->left=temp->left;
		int store1= n->val;
		n->val=temp->left->val;
		temp->left=NULL;
		n->left->val = store1;
		
	}
//	else	if(temp->right!=NULL && temp->left!=NULL)
//		{
//		//	rotateLeft(n);
//			rotateRight(n->right);
//		}
			
//	else if(temp->right != NULL && temp->left != NULL)
//	{
//		AVLTree::Node*store= temp->left;
//		temp->left=NULL;
//		n->right=NULL;
//		temp->left=n;
//		n->right=store;
//		*getRootNode()= temp->val;
//		 n=temp;
//		
//		
		
//		n->right=NULL;
//		n->right=temp->left;
//		temp->left=NULL;
		
		
//		n->left= temp->left;
//		n->left->right=temp->right;
//		temp->left=NULL;
//		temp->right=NULL;
//		int store1= n->val;
//		n->val= n->left->val;
//		n->left->val= store1;
//	}
	//*getRootNode()= temp->val; 
   	this->BinarySearchTree::updateNodeBalance(temp);
   //	AVLTree::balance(temp);
	return n;
	
}

AVLTree::Node* AVLTree::rotateRight(AVLTree::Node* & n)
{
		AVLTree::Node* temp= n;
		
	temp = n->left;	
	if(temp->left != NULL && temp->right == NULL)
	{
		n->right=temp->left;
		temp->left=NULL;
		int store1= n->val;
		n->val=temp->val;
		temp->val=n->right->val;
  	 	n->right->val=store1;
	}		   
	else if(temp->right != NULL && temp->left == NULL)
	{
		n->right=temp->right;
		int store1= n->val;
		n->val=temp->right->val;
		temp->right=NULL;
		n->right->val = store1;
	}
	else if(temp->left != NULL && temp->right != NULL)
	{
		cout<<"here";
		AVLTree::Node*store= temp->right;
		temp->right=NULL;
		n->left=NULL;
		temp->right=n;
		n->left=store;
	//	 this->BinarySearchTree::getRootNodeAddress() = temp;
		 cout<<"here";
		 n=temp;
		
//		n->right= temp->right;
//		n->right->left=temp->left;
//		temp->right=NULL;
//		temp->left=NULL;
//		int store1= n->val;
//		n->val= n->right->val;
//		n->right->val= store1;
	}
	else
   	this->BinarySearchTree::updateNodeBalance(temp);
   //	AVLTree::balance(temp);
	return n;
	
}

AVLTree::Node* AVLTree::leftRight(AVLTree::Node* & n)
{
	AVLTree::Node* temp;
	temp=n->left;
	n->left= this->rotateLeft(temp);
	return rotateRight(n);
}

AVLTree::Node* AVLTree::rightLeft(AVLTree::Node* & n)
{
	AVLTree::Node* temp;
	temp = n->right;
	n->right = rotateRight(temp);
	this->rotateLeft(n);
	return temp;
}

int AVLTree:: height1 (AVLTree::Node* & temp)
{
	int height = 0;
	if (temp != NULL)	
	{
		int leftH = height1 (temp->left);
		int rightH = height1 (temp->right);
		int maxH = fmax(leftH, rightH);
		height = maxH + 1;
	}
	return height;
}

int AVLTree:: diff (AVLTree::Node* & temp)
{
	int leftH= height1(temp->left);
	int rightH= height1(temp->right);
	int diff1 = leftH - rightH;
	return diff1;
	
}

//balancing AVL TREES

void AVLTree::balance(AVLTree::Node* & n)//
{
	if(n==NULL)
		return ;

	balance(n->right);
	balance(n->left);
	this->BinarySearchTree::updateNodeBalance(n);//scope operator
	//cout<<"avl balance"<<n->avlBalance;
	if( n->avlBalance > 1)
	{
		if(n->left->right!=NULL && n->left->left!=NULL)
		{
			//cout<<"was herre";this->rotateRight(n);
			this->rotateLeft(n->left);
		}
		
		this->rotateRight(n);
		
	}
	else if( n->avlBalance < -1)
	{
		
		if(n->right->right!=NULL && n->right->left!=NULL)
		{
		//	cout<<"here"<<n->avlBalance;
			this->rotateLeft(n);
			this->rotateRight(n->right);
		}
			
		this->rotateLeft(n);
	}
	 
		
//	iff node is null return
//	call balancce on temp->right and temp->left 
//	balance temp right 
//	balance temp left
//	update
	//if the node<-1 rotate left and if its greater rorate right 
	
}
/* 
AVL TREE TESTING RESULTS
************************
Test1: Test single left rotation
TEST PASSED
Test2: Test single right rotation
TEST PASSED
Test3: Test double left-right rotation
TEST PASSED
Test4: Test double right-left rotation
TEST PASSED
Test5: Test multiple rotations on insert
TEST FAILED
Test6: Test multiple rotations on remove
TEST FAILED

Press any key to continue . . .
*/


