#include<iterator> // for iterators
#include<vector>
#include <list>
#include <iostream>
using namespace std;
template <typename Container> void Write(Container& ds, ostream& os)
{
	//se cambio el iterador del documento por uno mas actual de c++ y stl
	for (auto it = ds.begin(); it != ds.end(); ++it)
	{
		os << *it << " ";
	}

}
int main(int argc, char* argv[])
{
	list<float> mylist;
	vector<float> myvector;
	for (int i = 0; i < 10; i++)
	{
		mylist.push_back(i);
		myvector.push_back(i + 50);
	}
	Write(mylist, cout);
	Write(myvector, cout);
	return 0;
}