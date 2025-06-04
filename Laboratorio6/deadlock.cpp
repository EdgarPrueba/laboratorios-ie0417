#include <mutex>
#include <thread>

/*Bloqueo (Lock): Una situación en la que un hilo adquiere un recurso 
(como un mutex) y evita que otros hilos accedan a ese recurso hasta que sea 
liberado.
Propósito: Asegurar que sólo un hilo a la vez pueda ejecutar una sección crítica
de código para evitar condiciones de carrera.
Deadlock: Una situación en la que dos o más hilos se bloquean mutuamente y 
ninguno puede continuar su ejecución porque cada uno está esperando queel otro 
libere un recurso.
Características:
- Ocurre cuando hay dependencia circular entre hilos.
- Ninguno de los hilos involucrados puede proceder.*/

std::mutex mtx1, mtx2;

// Definir dos mutex que representan dos recursos compartidos.
// threadA y threadB intentan adquirir ambos mutex pero en diferente orden,
// lo que puede causar un interbloqueo (deadlock) si ambos hilos se ejecutan simultáneamente.
// threadA: bloquea primero mtx1, espera, luego intenta bloquear mtx2.
// threadB: bloquea primero mtx2, espera, luego intenta bloquear mtx1.
// Si threadA y threadB se ejecutan al mismo tiempo, pueden quedar esperando
// indefinidamente a que el otro libere el recurso, ilustrando el problema del deadlock.

void threadA() {
    std::lock_guard<std::mutex> lock1(mtx1);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::lock_guard<std::mutex> lock2(mtx2);
}

void threadB() {
    std::lock_guard<std::mutex> lock2(mtx2);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::lock_guard<std::mutex> lock1(mtx1);
}

//Solo hace falta main para poder ejecutar