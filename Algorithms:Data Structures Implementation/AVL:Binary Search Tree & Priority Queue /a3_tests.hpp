#ifndef A3_TESTS_HPP
#define A3_TESTS_HPP

#define ASSERT_TRUE(T) if (!(T)) return false;
#define ASSERT_FALSE(T) if ((T)) return false;

#include <algorithm>
#include <sstream>
#include <queue>

#include "a3_priority_queue.hpp"
#include "a3_binary_search_tree.hpp"
#include "a3_avl_tree.hpp"

typedef PriorityQueue::DataType DataType;
typedef BinarySearchTree::DataType DataType;

DataType arrayMax( DataType* array, int n )
{
    DataType max = array[0];
    for( int i = 1; i < n; i++ )
        if( array[i] > max )
            max = array[i];
    return max;
}
//
//bool enqueue( int n, PriorityQueue &q, int cap, DataType* in, int &nin, DataType* out, int &nout )
//{
//    for( int i = 0; i < n; i++ )
//    {
//        bool full0 = nin == cap;
//    //    ASSERT_TRUE( q.size() == nin );
//        ASSERT_TRUE( q.full() == full0 );
//        ASSERT_TRUE( q.empty() == (nin == 0) );
//
//        ASSERT_TRUE( q.enqueue( out[nout-1] ) != full0 );
//        if( !full0 )
//            in[nin++] = out[--nout];
//
//        ASSERT_TRUE( q.size() == nin );
//        ASSERT_TRUE( q.full() == (nin == cap) );
//        ASSERT_TRUE( !q.empty() );
//
//        ASSERT_TRUE( q.max() == arrayMax( in, nin ) );
//    }
//    return true;
//}
//
//bool dequeue( int n, PriorityQueue &q, int cap, DataType* in, int &nin, DataType* out, int &nout )
//{
//    sort( in, in+nin );
//    for( int i = 0; i < n; i++ )
//    {
//        bool empty0 = nin == 0;
//        ASSERT_TRUE( q.size() == nin );
//        ASSERT_TRUE( q.full() == (nin == cap) );
//        ASSERT_TRUE( q.empty() == empty0 );
//
//        ASSERT_TRUE( q.dequeue() != empty0 );
//        if( !empty0 ) {
//            nin--;
//            // cout << "dequeued " << in[nin] << endl;
//        }
//
//        ASSERT_TRUE( q.size() == nin );
//        ASSERT_TRUE( !q.full() );
//        ASSERT_TRUE( q.empty() == (nin == 0) );
//
//        if( nin > 0 )
//            ASSERT_TRUE( q.max() == arrayMax( in, nin ) );
//    }
//    return true;
//}
//
//class PriorityQueueTest {
//public:
//	// New queue is valid
//  bool test1()
//  {
//    PriorityQueue q(16);
//    ASSERT_TRUE( q.empty() );
//    ASSERT_TRUE( !q.full() );
//    ASSERT_TRUE( q.size() == 0 );
//    return true;
//  }
//
//	// Enqueue one item and then dequeue it
//  bool test2()
//  {
//    PriorityQueue q(2);
//    ASSERT_TRUE( q.enqueue(10) );
//    ASSERT_TRUE( !q.empty() );
//    ASSERT_TRUE( !q.full() );
//    ASSERT_TRUE( q.size() == 1 );
//    ASSERT_TRUE( q.max() == 10 );
//
//    ASSERT_TRUE( q.dequeue() );
//    ASSERT_TRUE( q.empty() );
//    ASSERT_TRUE( !q.full() );
//    ASSERT_TRUE( q.size() == 0 );
//    return true;
//  }
//
//	// Enqueue too many
//  bool test3()
//  {
//    PriorityQueue q(3);
//    ASSERT_TRUE( q.enqueue(5) );
//    ASSERT_TRUE( q.max() == 5 );
//    ASSERT_TRUE( q.enqueue(3) );
//    ASSERT_TRUE( q.max() == 5 );
//    ASSERT_TRUE( q.enqueue(4) );
//    ASSERT_TRUE( q.max() == 5 );
//    ASSERT_TRUE( q.full() );
//
//    ASSERT_TRUE( !q.enqueue(7) );
//    ASSERT_TRUE( q.max() == 5 );
//    ASSERT_TRUE( q.full() );
//    return true;
//  }
//  
//  // Enqueue too many then dequeue too many
//bool test4()
//{
//    PriorityQueue q(3);
//    ASSERT_TRUE( q.enqueue(5) );
//    ASSERT_TRUE( q.max() == 5 );
//    ASSERT_TRUE( q.enqueue(3) );
//    ASSERT_TRUE( q.max() == 5 );
//    ASSERT_TRUE( q.enqueue(4) );
//    ASSERT_TRUE( q.max() == 5 );
//    ASSERT_TRUE( q.full() );
//
//    ASSERT_TRUE( !q.enqueue(7) );
//    ASSERT_TRUE( q.max() == 5 );
//    ASSERT_TRUE( q.full() );
//
//    ASSERT_TRUE( q.dequeue() );
//    ASSERT_TRUE( q.max() == 4 );
//    ASSERT_TRUE( q.dequeue() );
//    ASSERT_TRUE( q.max() == 3 );
//    ASSERT_TRUE( q.dequeue() );
//    ASSERT_TRUE( q.empty() );
//    ASSERT_TRUE( !q.full() );
//
//    ASSERT_TRUE( !q.dequeue() );
//    ASSERT_TRUE( q.empty() );
//    ASSERT_TRUE( !q.full() );
//    return true;
//}

//	// dequeue 5, enqueue 10, with repeat
//bool test5()
//{
//    const int cap = 15;
//    const int nout1 = 10;
//    int nout = nout1;
//    int out[nout1] = {9,5,23,1,3,15,14,1,5,3};
//    int nin = 0;
//    int in[nout1];
//
//    PriorityQueue q(cap);
//    ASSERT_TRUE( dequeue( 5, q, cap, in, nin, out, nout ) );
//    ASSERT_TRUE( enqueue( 10, q, cap, in, nin, out, nout ) );
//    
//    return true;
//}
//
//	// enqueue 50, dequeue 30, enqueue max, dequeue max, enqueue 25
//bool test6()
//{
//    const int cap = 64;
//    const int nout1 = 150;
//    int nin = 0;
//    int in[nout1];
//    int nout = nout1;
//    int out[nout1];
//    for( int i = 0; i < nout1; i++ ) out[i] = (i % 75) - 25;
//    random_shuffle( out, out+nout );
//
//    PriorityQueue q(cap);
//    ASSERT_TRUE( enqueue( 50, q, cap, in, nin, out, nout ) );
//    
//    ASSERT_TRUE( dequeue( 30, q, cap, in, nin, out, nout ) );
//    ASSERT_TRUE( enqueue( 10, q, cap, in, nin, out, nout ) );    
//    
//    ASSERT_TRUE( enqueue( 40, q, cap, in, nin, out, nout ) );
//    ASSERT_TRUE( dequeue( 70, q, cap, in, nin, out, nout ) );    
//    ASSERT_TRUE( enqueue( 25, q, cap, in, nin, out, nout ) );    
//    return true;
//}
//};

class BinarySearchTreeTest {
public:
bool insert_nodes(BinarySearchTree &tree, DataType *in, int nin)
{
  for(int i = 0; i < nin; i++) {
    ASSERT_TRUE(tree.insert(in[i]));
  }
  return true;
}

  // Traverse the tree in level-order to check if it was constructed properly
  // Output is as follows: "val1 val2 ... valN"
  // where vali is the value of node i, using level-order traversal
  std::string level_order(BinarySearchTree::Node* root)
  {
    // If no nodes, return an empty string.
    if (root == NULL) {
      return "";
    }
    
    std::stringstream ss;
    std::queue<BinarySearchTree::Node*> node_queue;
    node_queue.push(root);
    while (!node_queue.empty()) {
      BinarySearchTree::Node* cur_node = node_queue.front();
      node_queue.pop();
      ss << cur_node->val << " ";
      if (cur_node->left != NULL) {
        node_queue.push(cur_node->left);
      }
      if (cur_node->right != NULL) {
        node_queue.push(cur_node->right);
      }
    }
    
    std::string level_order_str = ss.str();
    // There is a trailing space at the end of the string; return everything
    // before this.
    return level_order_str.substr(0, level_order_str.size() - 1);
  }

	//  New tree is valid
bool test1()
{
    // We expect an empty tree at construction.
    std::string expected_tree_level_order = "";
    
    BinarySearchTree bst;
    ASSERT_TRUE(bst.root_ == NULL);
    ASSERT_TRUE(bst.size_ == 0 && bst.size() == 0);
    
    std::string tree_level_order = level_order(bst.root_);
    // Compare the tree's representation to the expected tree.
    ASSERT_TRUE(tree_level_order.compare(expected_tree_level_order) == 0)
    return true;
}

// Test a tree with one node
bool test2()
{
    // We expect a single root node with value "5" after everything.
    std::string expected_tree_level_order = "5";
    
    BinarySearchTree bst;
    ASSERT_TRUE(bst.insert(5));
    ASSERT_TRUE(!bst.remove(4));
    ASSERT_TRUE(bst.exists(5));
    ASSERT_TRUE(bst.max() == bst.min() && bst.max() == 5);
    ASSERT_TRUE(bst.size() == 1);
    
    std::string tree_level_order = level_order(bst.root_);
    // Compare the tree's representation to the expected tree.
    ASSERT_TRUE(tree_level_order.compare(expected_tree_level_order) == 0)
    return true;
}

// Insert, remove, and size on linear list formation with three elements
bool test3()
{
    // We expect a 2-node tree at the end of this function -- root node 10 and
    // a child node with value 8.
    std::string expected_tree_level_order = "10 8";
    
    BinarySearchTree bst;
    ASSERT_TRUE(bst.insert(10));
    ASSERT_TRUE(bst.insert(8));
    ASSERT_TRUE(bst.size() == 2);
    ASSERT_TRUE(bst.insert(6));
    ASSERT_TRUE(bst.size() == 3);

    ASSERT_TRUE(bst.remove(6));
    ASSERT_TRUE(bst.size() == 2);
    
    std::string tree_level_order = level_order(bst.root_);
    // Compare the tree's representation to the expected tree.
    ASSERT_TRUE(tree_level_order.compare(expected_tree_level_order) == 0)
    return true;
}

// Test removal of a node with one child
bool test4()
{
  std::string expected_tree = "3 2 6";
  
  BinarySearchTree bst;
  ASSERT_TRUE(bst.insert(3));
  ASSERT_TRUE(bst.insert(7));
  ASSERT_TRUE(bst.insert(6));
  ASSERT_TRUE(bst.insert(2));
  
  ASSERT_TRUE(bst.remove(7));
  ASSERT_TRUE(!bst.exists(7));
  ASSERT_TRUE(level_order(bst.root_).compare(expected_tree) == 0)
  return true;
}

// Insert multiple elements and remove till nothing remains
bool test5()
{
  std::string expected_tree = "";
  
  BinarySearchTree bst;  
  DataType in[] = {8, 3, 10, 15};
  
  ASSERT_TRUE(insert_nodes(bst, in, 4));
  ASSERT_TRUE(!bst.remove(9));
  ASSERT_TRUE(bst.remove(8));
  ASSERT_TRUE(bst.max() == 15);
  ASSERT_TRUE(bst.remove(15));
  ASSERT_TRUE(bst.max() == 10);
  ASSERT_TRUE(bst.min() == 3);
  ASSERT_TRUE(bst.remove(10));  
  ASSERT_TRUE(bst.remove(3));
  ASSERT_TRUE(bst.root_== NULL);
  ASSERT_TRUE(level_order(bst.root_).compare(expected_tree) == 0)
  return true;
}
 
 // Test removal of root node when both children of root have two children
bool test6()
{
  std::string expected_tree1 = "6 3 10 1 9 15";
  std::string expected_tree2 = "9 3 10 1 6 15";
  
  BinarySearchTree bst;
  DataType in[] = {8, 3, 10, 1, 6, 9, 15};
  
  ASSERT_TRUE(insert_nodes(bst, in, 7));
  ASSERT_TRUE(!bst.remove(12));
  ASSERT_TRUE(bst.remove(8));
  ASSERT_TRUE(level_order(bst.root_).compare(expected_tree1) == 0 ||
              level_order(bst.root_).compare(expected_tree2) == 0)
  return true;
}

// Test depth with many inserts & some removes.
bool test7()
{
  std::string expected_tree = "0 -5 10 -2 1 -4 -1";
  
  BinarySearchTree bst;
  ASSERT_TRUE(bst.insert(0));
  ASSERT_TRUE(bst.depth() == 0)
  ASSERT_TRUE(bst.insert(10));
  ASSERT_TRUE(bst.depth() == 1)
  ASSERT_TRUE(bst.insert(-5));
  ASSERT_TRUE(bst.depth() == 1)
  ASSERT_TRUE(bst.insert(-2));
  ASSERT_TRUE(bst.depth() == 2);
  ASSERT_TRUE(bst.insert(-4));
  ASSERT_TRUE(bst.depth() == 3);
  ASSERT_TRUE(bst.insert(-1));
  ASSERT_TRUE(bst.depth() == 3);
  ASSERT_TRUE(bst.insert(1));
  ASSERT_TRUE(bst.depth() == 3);
  ASSERT_TRUE(level_order(bst.root_).compare(expected_tree) == 0)
  return true;
}

// Lots of inserts and removes
bool test8()
{
  // In this case, it is the expected tree before removing all nodes.
  std::string expected_tree = "8 2 9 1 7 4 3 5 6";
  
  BinarySearchTree bst;
  DataType in[] = {8, 2, 7, 4, 5, 3, 1, 9, 6};
  int nin = 9;
  ASSERT_TRUE(insert_nodes(bst, in, nin));
  ASSERT_TRUE(level_order(bst.root_).compare(expected_tree) == 0)
  for(int i = 0; i<nin; ++i)
  {
    ASSERT_TRUE(bst.remove(in[i]));
  }
  ASSERT_TRUE(!bst.remove(in[0]));
  return true;
}
};

class AVLTreeTest {
public:
bool insert_nodes(AVLTree &tree, DataType *in, int nin)
{
  for(int i = 0; i < nin; i++) {
    ASSERT_TRUE(tree.insert(in[i]));
    
  }
  return true;
}

  // Traverse the tree in level-order to check if it was constructed properly
  // Output is as follows: "val1 val2 ... valN"
  // where vali is the value of node i, using level-order traversal
  std::string level_order(BinarySearchTree::Node* root)
  {
    // If no nodes, return an empty string.
    if (root == NULL) {
      return "";
    }
    
    std::stringstream ss;
    std::queue<BinarySearchTree::Node*> node_queue;
    node_queue.push(root);
    while (!node_queue.empty()) {
      BinarySearchTree::Node* cur_node = node_queue.front();
      node_queue.pop();
      ss << cur_node->val << " ";
      if (cur_node->left != NULL) {
        node_queue.push(cur_node->left);
      }
      if (cur_node->right != NULL) {
        node_queue.push(cur_node->right);
      }
    }
    
    std::string level_order_str = ss.str();
    // There is a trailing space at the end of the string; return everything
    // before this.
    return level_order_str.substr(0, level_order_str.size() - 1);
  }

	//  Test single left rotation
bool test1()
{
	std::string expected_tree_level_order = "2 1 3";	
	
 	AVLTree avl;
 	ASSERT_TRUE(avl.insert(1));
 	ASSERT_TRUE(avl.insert(2));
 	ASSERT_TRUE(avl.insert(3)); 	
 	std::string tree_level_order = level_order(avl.getRootNode());
 	ASSERT_TRUE(tree_level_order.compare(expected_tree_level_order) == 0)
 	ASSERT_TRUE(avl.insert(7));
 	ASSERT_TRUE(avl.insert(11));
 	expected_tree_level_order = "2 1 7 3 11";
 	tree_level_order = level_order(avl.getRootNode());
 	//cout << tree_level_order << endl;
 	ASSERT_TRUE(tree_level_order.compare(expected_tree_level_order) == 0)
    return true;
}

// Test single right rotation
bool test2()
{
	std::string expected_tree_level_order = "4 1 7";	
	
 	AVLTree avl;
 	ASSERT_TRUE(avl.insert(7));
 	ASSERT_TRUE(avl.insert(4));
 	ASSERT_TRUE(avl.insert(1)); 	
 	std::string tree_level_order = level_order(avl.getRootNode());
 	ASSERT_TRUE(tree_level_order.compare(expected_tree_level_order) == 0)
 	ASSERT_TRUE(avl.insert(6));
 	ASSERT_TRUE(avl.insert(5));
 	expected_tree_level_order = "4 1 6 5 7";
 	tree_level_order = level_order(avl.getRootNode());
 	ASSERT_TRUE(tree_level_order.compare(expected_tree_level_order) == 0)
    return true;
}

// Test double left-right rotation
bool test3()
{
	std::string expected_tree_level_order = "3 1 8";	
	
 	AVLTree avl;
 	ASSERT_TRUE(avl.insert(8));
 	ASSERT_TRUE(avl.insert(1));
 	ASSERT_TRUE(avl.insert(3)); 	
 	std::string tree_level_order = level_order(avl.getRootNode());
 	ASSERT_TRUE(tree_level_order.compare(expected_tree_level_order) == 0)
 	ASSERT_TRUE(avl.insert(5));
 	ASSERT_TRUE(avl.insert(6));
 	expected_tree_level_order = "3 1 6 5 8";
 	tree_level_order = level_order(avl.getRootNode());
 	ASSERT_TRUE(tree_level_order.compare(expected_tree_level_order) == 0)
    return true;
}

	//  Test double right-left rotation
bool test4()
{
	std::string expected_tree_level_order = "4 1 7";	
	
 	AVLTree avl;
 	ASSERT_TRUE(avl.insert(1));
 	ASSERT_TRUE(avl.insert(7));
 	ASSERT_TRUE(avl.insert(4)); 	
 	std::string tree_level_order = level_order(avl.getRootNode());
 	ASSERT_TRUE(tree_level_order.compare(expected_tree_level_order) == 0)
 	ASSERT_TRUE(avl.insert(11));
 	ASSERT_TRUE(avl.insert(9));
 	expected_tree_level_order = "4 1 9 7 11";
 	tree_level_order = level_order(avl.getRootNode());
 	ASSERT_TRUE(tree_level_order.compare(expected_tree_level_order) == 0)
    return true;
}

	//  Test multiple rotations on insert
bool test5()
{
	std::string expected_tree_level_order = "40 15 82 11 23 69 87 21 26 42";	
	
 	AVLTree avl;
 	DataType in[] = {11, 15, 26, 87, 40, 82, 69, 21, 23, 42};  
  	ASSERT_TRUE(insert_nodes(avl, in, 6));
  	
 	std::string tree_level_order = level_order(avl.getRootNode());
 	cout << tree_level_order << endl;//this->BinarySearchTree:: BinarySearchTree::print() const
 	ASSERT_TRUE(tree_level_order.compare(expected_tree_level_order) == 0)
    return true;
}
 
 // Test removal of root node when both children of root have two children
bool test6()
{
	std::string expected_tree_level_order = "23 15 40 11 21 26 42";	
	
 	AVLTree avl;
 	DataType in[] = {11, 15, 26, 87, 40, 82, 69, 21, 23, 42};  
  	ASSERT_TRUE(insert_nodes(avl, in, 10));
  	
  	ASSERT_TRUE(avl.remove(87));
  	ASSERT_TRUE(avl.remove(82));
  	ASSERT_TRUE(avl.remove(69));  	
 	std::string tree_level_order = level_order(avl.getRootNode());
 	ASSERT_TRUE(tree_level_order.compare(expected_tree_level_order) == 0)
 	
 	ASSERT_TRUE(avl.remove(11));
  	ASSERT_TRUE(avl.remove(15));
  	ASSERT_TRUE(avl.remove(21));  	
  	expected_tree_level_order = "40 23 42 26";	
  	tree_level_order = level_order(avl.getRootNode());
 	ASSERT_TRUE(tree_level_order.compare(expected_tree_level_order) == 0)
 	
 	ASSERT_TRUE(avl.remove(42));
  	expected_tree_level_order = "26 23 40";	
  	tree_level_order = level_order(avl.getRootNode());
 	ASSERT_TRUE(tree_level_order.compare(expected_tree_level_order) == 0)

	ASSERT_TRUE(avl.remove(26));
	ASSERT_TRUE(avl.remove(23));	
	ASSERT_TRUE(avl.remove(40));	
  	expected_tree_level_order = "";	
  	tree_level_order = level_order(avl.getRootNode());
 	ASSERT_TRUE(tree_level_order.compare(expected_tree_level_order) == 0)

    return true;
}
};
#endif
