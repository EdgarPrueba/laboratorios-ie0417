#include <mutex>

/*Mutex (Mutual Exclusion): Un mecanismo que permite a los hilos tomar turnos 
para acceder a una sección crítica del código, asegurando que solo un hilo a
la vez pueda ejecutarla.

- mutex: Se utiliza para bloquear y desbloquear acceso a una sección crítica.

- lock_guard: Asegura que el mutex se desbloquee automáticamente al salir del ámbito

*/
std::mutex mtx;

void safeIncrement(int &counter){
    std::lock_guard<std::mutex> lock(mtx);
    ++counter;
    //mtx se libera automáticamente cuando lock sale del ambito
}

int main (){
    int counter = 0;
    safeIncrement(counter);
    return 0;
}