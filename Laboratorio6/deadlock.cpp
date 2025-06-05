#include <mutex>
#include <thread>

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