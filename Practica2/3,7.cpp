#include <iostream>
#include <stdlib.h>
#include "libreria.h"

using namespace std;

int main()
{
    cout << MyNS::gnCount << '\n';
    cout << MyNS::Addition(2, 2)<<endl;
    return 0;
}