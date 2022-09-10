#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

using namespace std;

int contador = 0;

class punto {
public:
    int x;
    int y;
    int z;
    punto(int x, int y, int z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }
    punto() {
        this->x = 0;
        this->y = 0;
        this->z = 0;
    }
    void print() {
        cout << "x: " << x << " y: " << y << " z: " << z << endl;
    }
    //distancia del elemento 0 a todos los demas
    double distancia(punto p) {
        return sqrt(pow(p.x - this->x, 2) + pow(p.y - this->y, 2) + pow(p.z - this->z, 2));
    }
};

int main() {
    srand(time(NULL));
    vector<punto> puntos;
    for (int i = 0; i < 1000; i++) {
        puntos.push_back(punto(rand() % 1000, rand() % 1000, rand() % 1000));
    }
    punto p = punto(rand() % 1000, rand() % 1000, rand() % 1000);
    /* funcion print
    p.print();
    for (int i = 0; i < puntos.size(); i++) {
        puntos[i].print();
    }
    */
    //llamada a funcion distancia
    for (int i = 0; i < puntos.size(); i++) {
        cout <<contador <<"  distancia: " << p.distancia(puntos[i]) << endl;
        contador = contador + 1;
    }
}
