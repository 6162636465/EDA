#include <iostream>
#include <stdlib.h> 
using namespace std;

class CVector
{
private:
int *m_pVect, 
m_nCount, 
m_nMax, 
m_nDelta; 

void Init(int delta);
void Resize(); 
public:
void print();
CVector(int delta = 10); 
void Insert(int elem); 

};

CVector::CVector(int delta)
{
Init(delta);
}
//inicializar variables
void CVector::Init(int delta)
{
m_nCount = 0;
m_nMax = 0;
m_nDelta = delta;
m_pVect = NULL;
}
void CVector::Insert(int elem)
{
if( m_nCount == m_nMax ) 
Resize(); 
m_pVect[m_nCount++] = elem; 
}

void CVector::Resize()
{
int *pNewVect = new int[m_nMax + m_nDelta]; 
for(int i = 0; i < m_nCount; i++) 
pNewVect[i] = m_pVect[i];
delete [] m_pVect; 
m_pVect = pNewVect; 
m_nMax += m_nDelta; 
}

void CVector::print()
{
for(int i = 0; i < m_nCount; i++) 
cout << m_pVect[i] << " ";
cout << endl;
}


int main()
{
CVector v(5); 
for(int i = 0; i < 20; i++) 
v.Insert(i);
v.print();
}