#include<iostream>
using namespace std;
template <typename Type> class CVector
{
private:
	Type* m_pVect; // Pointer to the buffer
	int m_nCount, // Control how many elements are actually used
		m_nMax,
		m_nDelta; // 
	void Init(int delta);
	void Resize();
public:
	CVector(int delta = 10);
	void Insert(Type elem);
	void print();
};
// Class CVector implementation
template <typename Type> void CVector<Type>::Init(int delta)
{
	m_nCount = 0;
	m_nMax = 0;
	m_nDelta = delta;
	m_pVect = 0;
}
template <typename Type> void CVector<Type>::Resize()
{
	m_nMax += m_nDelta;
	Type* pNewVect = new Type[m_nMax];
	for (int i = 0; i < m_nCount; i++)
		pNewVect[i] = m_pVect[i];
	delete[] m_pVect;
	m_pVect = pNewVect;
}
template <typename Type> CVector<Type>::CVector(int delta)
{
	Init(delta);
}
template <typename Type> void CVector<Type>::Insert(Type elem)
{
	if (m_nCount == m_nMax)
		Resize();
	m_pVect[m_nCount++] = elem;
}
//se agrego la funcion imprimir
template <typename Type> void CVector<Type>::print()
{
	for (int i = 0; i < m_nCount; i++)
		cout << m_pVect[i] << " ";
	cout << endl;
}
int main()
{
	CVector<int> v(5);
	v.Insert(1);
	v.Insert(2);
	v.Insert(3);
	v.Insert(4);
	v.Insert(5);
	v.print();
}