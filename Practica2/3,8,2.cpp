#include <iostream>
#include <stdlib.h>

using namespace std;

float Addition(float a, float b)
{
  float c = a + b;
  return  c;
}
float Subtraction(float a, float b)
{
  float c = a - b;
  return  c;
}
float Multiplication(float a, float b)
{
float c = a * b;
  return  c;
}
float Division(float a, float b)
{
  float c = a / b;
  return  c;
}

int main()
{
  typedef float (*lpfnOperation)(float, float);
  //vector de punteros a Funciones
  lpfnOperation vpf[4] = {&::Addition, &::Subtraction,
  &::Multiplication, &::Division};
  float a, b, c; int opt;
  
  cin >> a >> b;

  cin >> opt;
 //del 0 al 3 de acuerdo a la funcion a llamar
  c = (*vpf[opt])(a, b);

  cout<<c<<endl;
}