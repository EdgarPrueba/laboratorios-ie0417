#include <mutex>
#include <condition_variable>

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