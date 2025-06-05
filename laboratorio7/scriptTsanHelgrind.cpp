#include <iostream>
#include <thread>

int counter = 0;

void increment() {
    for (int i = 0; i < 100000; i++) {
        // Posible punto de condición de carrera,
        // se podría modificar antes/después de ser cambiado por otro hilo.
        ++counter;
    }
}

int main() {
    // Al tener más de un hilo operando sobre la misma variable, se
    // pueden generar las condiciones de carrera.
    std::thread t1(increment);
    std::thread t2(increment);

    t1.join();
    t2.join();

    std::cout << "Counter: " << counter << std::endl;
    return 0;
}