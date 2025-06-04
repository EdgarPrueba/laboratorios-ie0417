/*El sistema consta de productores que generan tareas y consumidores que 
procesan estas tareas. Se utiliza un búfer compartido para almacenar las tareas,
y se emplean mecanismos de sincronización para asegurar que las tareas se 
manejen correctamente.*/

/*Definición de constantes y variables compartidas:
- BUFFER_SIZE: Tamaño del búfer compartido.
- buffer: Cola que actúa como el búfer compartido.
- mtx: Mutex para proteger el acceso al búfer.
- cv_producer, cv_consumer: Variables de condición para notificar a los productores y consumidores.
- empty_slots, full_slots: Semáforos que controlan el número de slots vacíos y llenos en el búfer.

Función del productor:
- Produce num_tasks tareas.
- Espera hasta que haya un slot vacío (empty_slots.acquire()).
- Entra en la sección crítica, agrega el item al búfer y notifica a un consumidor.
- Incrementa el contador de slots llenos (full_slots.release()).

Función del consumidor:
- Ejecuta en un bucle infinito, consumiendo tareas del búfer.
- Espera hasta que haya un ítem en el búfer (full_slots.acquire()).
- Entra en la sección crítica, consume el ítem del búfer y notifica a un productor.
- Incrementa el contador de slots vacíos (empty_slots.release()).
- Simula el tiempo de procesamiento.

Función principal (main):
- Crea y lanza hilos de productores y consumidores.
- Espera a que todos los productores terminen (join).
- Permite que los consumidores terminen de procesar (sleep).*/

#include <iostream>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <semaphore>
#include <chrono>

using namespace std;

constexpr int BUFFER_SIZE = 10;
std::queue<int> buffer;
std::mutex mtx;
std::condition_variable cv_producer, cv_consumer;
std::counting_semaphore<BUFFER_SIZE> empty_slots(BUFFER_SIZE); //Inicializando con BUFFER_SIZE
std::counting_semaphore<BUFFER_SIZE> full_slots(0); //iniciando con cero

void producer(int id, int num_tasks){
    for (int i = 0; i <num_tasks; ++i){
        int item = id*100 + i;
        empty_slots.acquire();

        //Critical section
        {
            std::lock_guard<std::mutex> lock(mtx);
            buffer.push(item);
            std::cout << "Producer" << id <<" produced-item" << item << std::endl;
        }

        full_slots.release(); // Incrementa el contador de slots llenos 
        cv_consumer.notify_one(); // Notifica a un consumidor
    }

}

void consumer(int id) {
    while (true){
        full_slots.acquire(); // Decrementa el contador de slots llenos
        std::unique_lock<std::mutex> lock(mtx);
        // Espera hasta que haya un item en el buffer 
        cv_consumer.wait(lock, [] { return !buffer.empty();});

        int item= buffer.front();
        buffer.pop();
        std::cout <<"Consumer " << id <<" consumed item" << item << std::endl;

        lock.unlock();
        empty_slots.release();// Incrementa el contador de slots vacíos 
        cv_producer.notify_one(); // Notificar a un productor

        // Simulate processing time
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int main(){
    //Definir el número de productores, consumidores y tareas por productor
    const int num_producers = 2;
    const int num_consumers= 3;
    const int num_tasks_per_producer = 10;

    //Declarar vectores para los hilos productores y consumidores
    std::vector<std::thread> producers, consumers;

    //Crear los hilos productores
    for (int i = 0; i < num_producers; ++i){
        producers.emplace_back(producer, i, num_tasks_per_producer);
    }

    //Crear los hilos consumidores
    for (int i = 0; i < num_consumers; ++i){
        consumers.emplace_back(consumer, i);
    }

    for (auto& producers_thread: producers){
        producers_thread.join();
    }

    //Espera 2 segundos para que los consumidores procesen los últimos elementos
    std::this_thread::sleep_for(std::chrono::seconds(2));

    //los productores han terminado
    std::cout <<"All prodecers are finished" << std::endl;
    return 0;
}