#include <iostream>
#include <vector>

/*Si múltiples hilos ejecutan este bucle simultáneamente, pueden leer
el mismo valor de counter antes de que se actualice, resultando en
un conteo incorrecto.
 Sin sincronización, los hilos pueden corromper la estructura interna
de myList, causando un comportamiento indefinido.*/

int counter = 0;

int main (){
    for (int i= 0; i < 1000; ++i){
        ++counter; //Incremento no atómico
    }
    std::vector<int> myList;
    //Hilo 1
    myList.push_back(1);
    //Hilo 2
    myList.push_back(2);
}