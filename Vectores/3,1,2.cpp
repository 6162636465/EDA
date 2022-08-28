//3.1.2

#include <iostream>
#include <stdlib.h> 
using namespace std;


int* gpVect = NULL; // Dynamic buffer to save the elements int
int gnCount = 0; // Counter to know the number of used elements int
int gnMax = 0;

void Resize()
{
  const int delta = 10;
    int *pTemp, i;
    pTemp = new int[gnMax + delta]; // Alloc a new vector
    for(i = 0 ; i < gnMax ; i++) // Transfer the elements
    pTemp[i] = gpVect[i]; // we can also use the function memcpy
    delete [ ] gpVect; // delete the old vector
    gpVect = pTemp; // Update the pointer
    gnMax += delta; // The Max has to be increased by delta
}

void Insert(int elem)
{

    if( gnCount == gnMax )
  {// There is no space at this moment for elem
    Resize(); // Resize the vector before inserting elem
    }
  gpVect[gnCount++] = elem;
  
}

void print()
{
    int* pTemp, i;
    for (i = 0; i < gnMax; i++)
    {
        cout << gpVect[i] << "," ;
    }
}
int main()
{
    Insert(1);
    Insert(2);
    Insert(3);
    Insert(4);
    Insert(5);
    Insert(6);
    print();
}