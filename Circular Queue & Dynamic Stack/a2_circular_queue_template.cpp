// Shilpan Shah 20603389  Khalid Berry 20619265 
#include <iostream>
#include "a2_circular_queue_template.hpp"


template<>
const int CircularQueue<int>::EMPTY_QUEUE = -999;

template<>
const std::string CircularQueue<std::string>::EMPTY_QUEUE = "Empty";

template<>
const double CircularQueue<double>::EMPTY_QUEUE = -999.9;

template<class T>
CircularQueue<T>::CircularQueue()
{
	 size_=0;
	 capacity_=16;
	head_=-1;
	tail_=0;
	 items_ = new T[16]; // items is the name of the array. stackitem is typedef. this is hoow you declare array 
	
}
template<class T>// TEMPLATE IS A CLASS. you are passing in the template class. 
CircularQueue<T>::CircularQueue(unsigned int capacity)
{
	if(capacity_<=0)
		{
		}
	
	capacity_= capacity;
	items_ = new T[capacity_]; 
	  
	size_=0;
	 head_=-1;
	 tail_=0;
}
template<class T>
CircularQueue<T>::~CircularQueue()
{
	delete [] items_; // deletes the array of items. 
}
template<class T>
bool CircularQueue<T>::empty() const
{    
	if(size_==0)
		return true;
	
	return false; 
}
template<class T>
bool CircularQueue<T>::full() const
{
		if(size_==capacity_)
		return true;
	
	return false; 
}
template<class T>
int CircularQueue<T>::size() const
{  
	return size_;
}

template<class T>
bool CircularQueue<T>::enqueue(T value)
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
template<class T>

T CircularQueue<T>::dequeue()
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

template<class T>
T CircularQueue<T>::peek() const
{
	if (empty())
			return EMPTY_QUEUE;
	else{
		return items_[head_];
	}
}
template<class T>

void CircularQueue<T>::print() const
{
}

/////////////////////////////////////////////////////////////////////////


#define ASSERT_TRUE(T) if (!(T)) return false;
#define ASSERT_FALSE(T) if ((T)) return false;

using namespace std;

class CircularQueueTest
{
public:
	// New queue is valid
    bool test1() {
	      CircularQueue <int> queue;
	      ASSERT_TRUE(queue.empty() == true)
	      ASSERT_TRUE(queue.full() == false)
	      ASSERT_TRUE(queue.size() == 0)
	      ASSERT_TRUE(queue.size_ == 0)
	      ASSERT_TRUE(queue.capacity_ == 16)     
	      return true;
    }


	// Enqueue one item and then dequeue it
    bool test2() {
	      CircularQueue <double> queue;
	      ASSERT_TRUE(queue.enqueue(10) == true)
	      ASSERT_TRUE(queue.empty() == false)
	      ASSERT_TRUE(queue.full() == false)
	      ASSERT_TRUE(queue.size_ == 1)
	      ASSERT_TRUE(queue.head_ == 0)
	      ASSERT_TRUE((queue.tail_ == 1) || (queue.tail_ == 2))
	      ASSERT_TRUE(queue.dequeue() == 10);
	      ASSERT_TRUE(queue.empty() == true)
	      ASSERT_TRUE(queue.size_ == 0)
	      return true;
    }

	// Enqueue two items and then dequeue three
    bool test3() {
	      CircularQueue <string> queue;
	      
	      ASSERT_TRUE(queue.enqueue("XI") == true)     
	      ASSERT_TRUE(queue.empty() == false)
	      ASSERT_TRUE(queue.full() == false)
	      ASSERT_TRUE(queue.size_ == 1)
	      ASSERT_TRUE(queue.head_ == 0)
	      ASSERT_TRUE((queue.tail_ == 1) || ((queue.tail_ == 2)))
	      ASSERT_TRUE(queue.dequeue() == "XI");
	      ASSERT_TRUE(queue.empty() == true)
	      ASSERT_TRUE(queue.size_ == 0)


	      ASSERT_TRUE(queue.peek() == CircularQueue<string>::EMPTY_QUEUE);  
	      ASSERT_TRUE(queue.dequeue() == CircularQueue<string>::EMPTY_QUEUE);  

	      return true;
	  }
    
};


/////////////////////////////////////////////////////////////////////////////	
string get_status_str(bool status)
{
    return status ? "TEST PASSED" : "TEST FAILED";
}

int main() {
    CircularQueueTest queue_test;

	string queue_descriptions[10] =	 {
      "Test1: int template",
      "Test2: double template",
      "Test3: string template",
	};
	bool queue_test_results[10];
    queue_test_results[0] = queue_test.test1();
    queue_test_results[1] = queue_test.test2();
    queue_test_results[2] = queue_test.test3();
   

cout << "CIRCULAR QUEUE TESTING RESULTS \n";
	cout << "****************************** \n";
	for (int i = 0; i < 3; ++i) {
    	cout << queue_descriptions[i] << endl << get_status_str(queue_test_results[i]) << endl;
	}
	cout << endl;
    system("pause");
	
}

/* 
CIRCULAR QUEUE TESTING RESULTS
******************************
Test1: int template
TEST PASSED
Test2: double template
TEST PASSED
Test3: string template
TEST PASSED

Press any key to continue . . .
*/
