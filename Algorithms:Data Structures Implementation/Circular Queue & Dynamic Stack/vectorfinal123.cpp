// Shilpan Shah 20603389  Khalid Berry 20619265 
#include <iostream> 
#include <string>
#include <vector> 
using namespace std;

int main()
{
	int size= 20;
	int newSize=30;
				
	vector<int> newvector;
	newvector.reserve(size);
	
	//Test for reserve()
	if(newvector.capacity() == size)
	{
		newvector.resize(newSize);
	
		//Test for resize()
		if(newvector.capacity()==newSize)
			cout << "TEST1 PASSED" << endl;
		else
			cout << "TEST1 FAILED" << endl;
	}
	else
		cout << "TEST1 FAILED" << endl;
	
	// Test 2				
		
	vector<int> myvector;
	myvector.reserve(4);
	
	//Test for automatic reallocation		
	for(int i=0;i<5;i++)
	{
		myvector.push_back(40*i);
	}
	
	//Test for end()
	myvector.insert(myvector.end(),5);
	
	if(myvector.capacity() == 8) // The capacity is automatically double
	{
		//Test for erase(), begin() and operator[]
		for(int i=0;i<2;i++)
		{
			myvector.erase(myvector.begin());
		}
		
		if(myvector.at(3) == 5)
		{
			//Test for pop_back()
			myvector.pop_back();
		
			if(myvector.at(0)==80 && myvector.at(1)==120 && myvector.operator[](2)==160)
				cout<<"TEST2 PASSED"<<endl;
			else
				cout<<"TEST2 FAILED"<<endl;
		}
		else
			cout<<"TEST2 FAILED"<<endl;
	}
	else
		cout<<"TEST2 FAILED"<<endl;
				
	///////////////
	vector<int> AVector;
			AVector.reserve(10);
			//AVector.capacity()=size;
			for(int i=0;i<5;i++)
			{
				AVector.push_back(22);
			}
			for(int i=0;i<3;i++)
			{
				AVector.pop_back();
			}
			AVector.capacity();
			
			if(AVector.at(1)==22 && AVector.capacity()==10)
						cout<<"TEST3 PASSED"<<endl;
						else
						cout<<"TEST3 FAILED"<<endl;
			
			
}
				
//Result:
			
/*
TEST1 PASSED
TEST2 PASSED
TEST3 PASSED

--------------------------------
Process exited after 0.8613 seconds with return value 0
Press any key to continue . . .
*/				
				
				
				
	
