// Shilpan Shah 20603389  Nikhil Keswani 20611258 
#include <iostream>
#include <cmath>
#include "a3_priority_queue.hpp"
using namespace std;

typedef PriorityQueue::DataType DataType;



PriorityQueue::PriorityQueue(unsigned int capacity)
{
	heap_ = new DataType[capacity];
	capacity_ = capacity;
	size_ = 0; 
}

PriorityQueue::~PriorityQueue()
{
	delete [] heap_;  
}

bool PriorityQueue::empty() const
{    
	return (size_ == 0);	
}

bool PriorityQueue::full() const
{
		return (size_ == capacity_);
}

unsigned int PriorityQueue::size() const
{  
	return size_;
}

DataType PriorityQueue::max() const
{  
//	if(empty())
//		return 0; 
//	int max = heap_[1];
//	for(int i=1; i<=size_; i++)
//	{
//		if(heap_[i] > max)
//			max = heap_[i];
//	}

	return heap_[1];
}

bool PriorityQueue::enqueue(DataType val)
{
	if (empty())
	{
		heap_[1] = val;
		size_++;
		return true;
	}
	if (full())
		return false;
	int index = size_ + 1;
	heap_[index] = val;
	while (val > heap_[index/2])
	{
		int temp = heap_[index/2];
		heap_[index/2] = val;
		heap_[index] = temp;
		index = (index/2);
	}
	size_++;
	return true;
}

bool PriorityQueue::dequeue()
{

	if (empty())
		return false;
	int index = 1;// 
	
//	int temp= heap_[1];
	heap_[1]= heap_[size_];
//	heap_[size_]= temp;
	size_--;
	
while ((size_ >= 2*index) && ((heap_[index] < heap_[index*2]) || (heap_[index] < heap_[index*2+1]))) //while indexed node is not a leaf and is smaller than a child

	{
		if(heap_[2*index]> heap_[index*2+1] && heap_[2*index]> heap_[index])// if left node is greater than right node and root node
		{
			int temp1= heap_[2*index];
			heap_[2*index]= heap_[index];// left node will store the value of the root node. 
			heap_[index]=temp1;// root will ger the value of the left node, since it is bigger than the root node
			index= index*2;//move the index over to the node left node and checks again. 
		}
		
		else if(heap_[2*index]<= heap_[index*2+1] && heap_[index*2+1]>= heap_[index])// if right node is greater than left node and root node)
		{
			int temp2= heap_[2*index+1];
			heap_[2*index+1]= heap_[index];// right node
			heap_[index]=temp2;
			index= index*2 +1;
		}
		else if( heap_[index]>= heap_[index*2+1] && 	heap_[index]>= heap_[index*2])
			return true;
		
	}
	
	return true;

}


void PriorityQueue::print() const
{
	if(!empty())
	{
		for(int i=1;i<size_; i++)
		{
			cout<< heap_[i]<<endl;
		}
			}

}
/*
PRIORITY QUEUE TESTING RESULTS
******************************
Test1: New queue is valid
TEST PASSED
Test2: Enqueue one item and then dequeue it
TEST PASSED
Test3: Enqueue too many
TEST PASSED
Test4: Enqueue too many then dequeue too many
TEST PASSED
Test5: Dequeue 5, enqueue 10, with repeat
TEST PASSED
Test6: Lots of enqueues and dequeues
TEST PASSED

Press any key to continue . . .
*/

