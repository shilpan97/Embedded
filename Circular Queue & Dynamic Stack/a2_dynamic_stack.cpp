// Shilpan Shah 20603389  Khalid Berry 20619265 
#include <iostream>
#include "a2_dynamic_stack.hpp"

using namespace std;

typedef DynamicStack::StackItem StackItem;  // for simplicity
const StackItem DynamicStack::EMPTY_STACK = -999;

DynamicStack::DynamicStack()
{
	 size_=0;
	 init_capacity_=16;
	 capacity_=16;
	 items_ = new StackItem[capacity_]; // items is the name of the array. stackitem is typedef. this is hoow you declare array 
	
}

DynamicStack::DynamicStack(unsigned int capacity)
{
	//add what if capacity is <0
	capacity_= capacity;
	items_ = new StackItem[capacity]; 
	size_=0;
	 init_capacity_=capacity;
}

DynamicStack::~DynamicStack()
{
	delete [] items_; // deletes the array of items. 
}

bool DynamicStack::empty() const
{
	if(size_==0)
		return true;
	
	return false; 
}

int DynamicStack::size() const
{
	return size_;
}

void DynamicStack::push(StackItem value)
{

	if (size_==capacity_) 
	{
		 capacity_=capacity_*2;
			 StackItem* temp = new StackItem[capacity_]; //Declaring the new Dynamic stack as temp

		 for(int i=0; i<size_;i++)
		  	temp[i]= items_[i];
	
	//	delete [] items_; // deletes the array of items. 
			temp[size_]=value;
		size_++;
		items_=temp;
	
	}
	else
	{	items_[size_] = value;
		size_++;
	}
}

StackItem DynamicStack::pop()
{
	//if original is empty
		if (empty())
			return EMPTY_STACK;
		
	 //if original needs to be reduced = copy stack over to smaller array and delete old stack size--
	 	size_--;

		if( size_<=capacity_/4 && capacity_/2 >= init_capacity_ )
		{
				 	int val=items_[size_];
		capacity_/=2;
	 
	 StackItem* temp = new StackItem[capacity_]; //Declaring the new Dynamic stack as temp

		 for(int i=0; i<size_;i++)
		  	temp[i]= items_[i];
	//if size ok, take last value and size --
			items_=temp;
			return val;
		}
	
// deletes the array of items. 
//	StackItem temp = items_[size_-1];
//
//	items_[size_-1]=0;
//	size_--;
	

	
	return items_[size_];
}

StackItem DynamicStack::peek() const
{
		if (empty())
			return EMPTY_STACK;
	
		return items_[size_-1];
}

void DynamicStack::print() const
{

	for(int i=0; i<size_; i++)
		cout<<*(items_+i)<< endl;
for(int i=size_-1;i>=0;i--)
	{
		cout << items_[i] << endl;
	}
	
}
/*
DYNAMIC STACK TESTING RESULTS
*****************************
Test1: New stack is valid
TEST PASSED
Test2: Push one item and then pop it
TEST PASSED
Test3: Simple push, pop, and peek test
TEST PASSED
Test4: Push 4 items, peek 5 items and then pop 5 items
TEST PASSED
Test5: Full stack doubles properly
TEST PASSED
Test6: Full stack doubles, then full stack doubles
TEST PASSED
Test7: Push a lot, pop a lot, no halving
TEST PASSED
Test8: Push a lot, pop a lot, halving
TEST PASSED
Test9: Lots of pushes, peeks, and pops
TEST PASSED
Test10: Lots of pushes, peeks, and pops
TEST PASSED


*/ 
