#include <mutex>
#include <condition_variable>

/*Mecanismos de sincronización comunes
Variables de condición: Permiten que un hilo espere (bloquee) hasta que otro 
hilo señale (notifique) que una condición particular se ha cumplido.

- std::condition_variable: Utilizada para esperar y notificar cambios de estado.

*/

std::mutex mtx;
std::condition_variable cv;

bool ready = false;

void waitingThead(){
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [] {return ready;}); //Espera hasta que ready sea true
    //Continuar ejecución.
}

void notifyingTheard(){
    std::lock_guard<std::mutex> lock(mtx);
    ready = true;
    cv.notify_one(); //Notifica a uno de los hilos en espera
}

int main (){
    waitingThead();
    notifyingTheard();
    return 0;
}