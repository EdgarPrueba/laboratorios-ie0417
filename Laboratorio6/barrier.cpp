#include <barrier>
#include <iostream>

std::barrier sync_point(5); //barrera para 5 hilos
void threadFuntion(){
    // Realizar tareas antes de la barrera
    sync_point.arrive_and_wait(); //Esperar a que todos los hilos lleguen aquí
    // Continuar después de que todos los hilos hayan sincronizado

}

int main (){
    threadFuntion();
    return 0;
}