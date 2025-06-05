#include <iostream>

int main(){
    int* q = new int[10];
    // No se libera la memoria asignada

    int* p = new int[10];
    std::cout << p[0];  // p[0] no está inicializado, pero se intenta usar
    delete p;
    
    int array[10];
    array[10] = 5;  // Acceso fuera de los límites del array
    return 0;
}