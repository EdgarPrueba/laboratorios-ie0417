#include <barrier>
/*Mecanismos de sincronización comunes
Barreras de sincronización (Barriers): Permiten que un grupo de hilos espere 
hasta que todos los  hilos en el grupo hayan alcanzado un punto de 
sincronización común antes de continuar.

- std::barrier: Disponible desde C++20, sincroniza un grupo de hilos

*/

std::barrier sync_point(5); //barrera para 5 hilos
void threadFuntion(){
    // Realizar tareas antes de la barrera
    sync_point.arrive_and_wait(); //Esperar a que todos los hilos lleguen aquí
    // Continuar después de que todos los hilos hayan sincronizado

}

int main (){
    threadFuntion();
}