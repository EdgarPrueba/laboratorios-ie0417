#include <semaphore>
/*Un contador que controla el acceso a uno o más recursos compartidos, 
permitiendo que un número limitado de hilos accedan a la sección crítica
al mismo tiempo.
- counting_semaphore: Disponible desde C++20, permite definir un semáforo con un 
contador inicial.*/

std::counting_semaphore<1> sem(1);

void accessResource(){
    sem.acquire();
    sem.release();
}
int main() {
    accessResource();
    return 0;
}