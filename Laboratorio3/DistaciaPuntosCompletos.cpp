#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <ctime> 

using namespace std;
unsigned t0, t1, t2, t3, t4, t5;


int contador = 0;
int contadorB = 0;
int contadorC = 0;

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

int main()
{
    t0 = clock();
    srand(time(NULL));
    vector<punto> puntos;
    for (int i = 0; i < 1000; i++) {
        puntos.push_back(punto(rand() % 1000, rand() % 1000, rand() % 1000));
    }
    punto p = punto(rand() % 1000, rand() % 1000, rand() % 1000);

    for (int i = 0; i < puntos.size(); i++) {
        
        /*cout << contador << "  distancia: " << p.distancia(puntos[i]) << endl;
        contador = contador + 1;*/
        p.distancia(puntos[i]);
    }
    t1 = clock();

    double time = (double(t1 - t0) / CLOCKS_PER_SEC);
    cout << "tiempo de Ejecucion de 1000 elementos: " << time << endl;

    //_______________________________________ 10000 
    t3 = clock();
    vector<punto> puntos3;
    for (int i = 0; i < 10000; i++) {
        puntos3.push_back(punto(rand() % 10000, rand() % 10000, rand() % 10000));
    }

    punto p2 = punto(rand() % 10000, rand() % 10000, rand() % 10000);

    for (int i = 0; i < puntos3.size(); i++)
    {/*
        cout << contadorB << "  distancia: " << p2.distancia(puntos3[i]) << endl;
        contadorB = contadorB + 1;*/

        p2.distancia(puntos3[i]);

    }
    t4 = clock();
    double time4 = (double(t4 - t3) / CLOCKS_PER_SEC);
    cout << "tiempo de Ejecucion de 10000 elementos: " << time4 << endl;

    //___________________________________________ 50000 
    t2 = clock();
    vector<punto> puntos2;
    for (int i = 0; i < 50000; i++) {
        puntos2.push_back(punto(rand() % 50000, rand() % 50000, rand() % 50000));
    }

    punto p1 = punto(rand() % 50000, rand() % 50000, rand() % 50000);

    for (int i = 0; i < puntos2.size(); i++) 
    {
        /*
        cout << contadorC << "  distancia: " << p1.distancia(puntos2[i]) << endl;
        contadorC = contadorC + 1;*/

       p1.distancia(puntos2[i]);
    }
    t3 = clock();

    double time2 = (double(t3 - t2) / CLOCKS_PER_SEC);
    cout << "tiempo de Ejecucion de 50000 elementos: " << time2 << endl;
}