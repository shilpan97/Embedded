// Shilpan Shah 20603389  Khalid Berry 20619265 
#include <iostream>
#include "a2_circular_queue.hpp"

typedef CircularQueue::QueueItem QueueItem;
const QueueItem CircularQueue::EMPTY_QUEUE = -999;

CircularQueue::CircularQueue()
{
	 size_=0;
	 capacity_=16;
	head_=-1;
	tail_=0;
	 items_ = new QueueItem[16]; // items is the name of the array. stackitem is typedef. this is hoow you declare array 
	
}

CircularQueue::CircularQueue(unsigned int capacity)
{
	if(capacity_<=0)
		{
		}
	
	capacity_= capacity;
	items_ = new QueueItem[capacity_]; 
	  
	size_=0;
	 head_=-1;
	 tail_=0;
}

CircularQueue::~CircularQueue()
{
	delete [] items_; // deletes the array of items. 
}

bool CircularQueue::empty() const
{    
	if(size_==0)
		return true;
	
	return false; 
}

bool CircularQueue::full() const
{
		if(size_==capacity_)
		return true;
	
	return false; 
}

int CircularQueue::size() const
{  
	return size_;
}

bool CircularQueue::enqueue(QueueItem value)
{
	if(full())
	return false;
	if( head_==-1)// empty
	{
		head_=0;
		items_[tail_]= value;
		tail_++;// tail is always one after the last element
		size_++;
		return true;
		}
	
	else
	{
		//tail= capacity-1 case
		if( tail_==capacity_-1)
		{
		items_[tail_]= value;
		tail_=0; // reached full. tail is first one now. 
		size_++;
		return true;
		}
		else
		{
		//general casae
		items_[tail_]= value;
		tail_++;
		size_++;
		return true;
		}
	 } 
}

QueueItem CircularQueue::dequeue()
{
	if (empty())
			return EMPTY_QUEUE;
			
	else
	{
		int temp=head_;
		size_--;
		if( head_== capacity_-1)
		head_=0;
		else 
		head_++;
		return items_[temp];
		
		
	}
}

QueueItem CircularQueue::peek() const
{
	if (empty())
			return EMPTY_QUEUE;
	else{
		return items_[head_];
	}
}

void CircularQueue::print() const
{
}
/*
CIRCULAR QUEUE TESTING RESULTS
******************************
Test1: New queue is valid
TEST PASSED
Test2: Enqueue one item and then dequeue it
TEST PASSED
Test3: Enqueue two items and then dequeue three
TEST PASSED
Test4: Enqueue 16 items, peek 3 items and then dequeue 18 items
TEST PASSED
Test5: Enqueue items onto a full queue returns false
TEST PASSED
Test6: Queue is circular for enqueue()
TEST PASSED
Test7: Fill queue, then empty queue
TEST PASSED
Test8: Queue is circular for dequeue()
TEST PASSED
Test9: Lots of enqueues, dequeues, peeks, all valid
TEST PASSED
Test10: Lots of enqueues, dequeues, peeks, not all valid
TEST PASSED
*/ 
