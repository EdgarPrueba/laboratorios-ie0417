# Universidad de Costa Rica  
**Escuela de Ingeniería Eléctrica**  
**IE-0417 – Diseño de Software para Ingeniería**  
**Laboratorio 6: Programación concurrente, sincronización y multihilo en C++**
**Integrantes:**  
- Diego Alfaro (C20259)  
- Edgar Alvarado (C10351)  
- Jean Zúñiga (C18767)  
---

## Comando de ejecución:
g++-12 -std=c++20 <nombre_archivo.cpp> -o <nombre_ejecutable.exe>

## `mutex.cpp` — Exclusión mutua en C++ (Mutex)

Este ejemplo demuestra el uso de **mutexes** (*mutual exclusion*) en C++ para proteger secciones críticas del código cuando múltiples hilos acceden a recursos compartidos.

---

## ¿Qué es un mutex?

Un **mutex** es un mecanismo de sincronización que garantiza que **solo un hilo a la vez** pueda acceder a una sección crítica del código. Se utiliza para **prevenir condiciones de carrera** y asegurar la **integridad de los datos compartidos**.

---

##  Conceptos 

| Elemento        | Descripción                                                                 |
|------------------|-----------------------------------------------------------------------------|
| `std::mutex`     | Objeto que permite bloquear/desbloquear acceso a secciones críticas.       |
| `std::lock_guard`| Wrapper RAII que bloquea el mutex en su construcción y lo libera al salir del ámbito automáticamente. |


##  `raceCondition.cpp` — Condiciones de carrera en C++

Este ejemplo ilustra el concepto de **condición de carrera** (*race condition*), un problema común en programación concurrente, donde múltiples hilos acceden y modifican recursos compartidos de forma no sincronizada, provocando comportamientos impredecibles o errores silenciosos.

---

##  Descripción general

Una **race condition** ocurre cuando:

- Varios hilos acceden a una misma variable o recurso compartido.
- Al menos uno de los accesos es una escritura.
- No existe un mecanismo de sincronización que regule dicho acceso.

Esto puede provocar resultados inconsistentes o la corrupción de estructuras de datos.


##  `semaforos.cpp` — Sincronización con `std::counting_semaphore` en C++

Este archivo muestra el uso de **semáforos contadores** (`counting_semaphore`), una herramienta esencial de sincronización que permite controlar el acceso concurrente a uno o más recursos compartidos por parte de múltiples hilos.

---

##  Descripción general

Un **semaforo** actúa como un contador que limita el número de hilos que pueden ingresar a una sección crítica al mismo tiempo. 

Este mecanismo es útil cuando:

- Se desea permitir acceso concurrente limitado a recursos (ej. hasta 3 hilos simultáneos).
- Se necesita implementar lógica de **pool de conexiones**, **control de acceso a buffers**, o **limitación de concurrencia**.

---

### Declaración del semáforo

```cpp
#include <semaphore>

std::counting_semaphore<1> sem(1);
```

## `variableCondicion.cpp` — Sincronización con `std::condition_variable` en C++

Se presenta el uso de **variables de condición** (`condition_variable`), un mecanismo de sincronización que permite que los hilos esperen (queden bloqueados) hasta que otro hilo notifique que se ha cumplido una condición específica.

Es una técnica para evitar el uso activo del CPU (busy waiting) y garantizar que los hilos actúen solo cuando deben hacerlo.

---

## Descripción general

Las **variables de condición** permiten la coordinación entre hilos mediante:

-  **Espera pasiva**: un hilo se bloquea hasta que la condición requerida se cumple.
-  **Notificación activa**: otro hilo modifica el estado y despierta a los hilos que esperan.

Este patrón es común en estructuras como **productor-consumidor**, **trabajadores dependientes de recursos**, o **sincronización por eventos**.

---

### Variables globales

```cpp
std::mutex mtx;
std::condition_variable cv;
bool ready = false;
```


## variableCondicion.cpp
Mecanismos de sincronización comunes
Variables de condición: Permiten que un hilo espere (bloquee) hasta que otro
hilo señale (notifique) que una condición particular se ha cumplido.

- std::condition_variable: Utilizada para esperar y notificar cambios de estado.


## `deadlock.cpp` — Demostración de **Interbloqueo (Deadlock)** en C++

Este archivo muestra un ejemplo sobre una de las situaciones más problemáticas en programación concurrente: el **interbloqueo** o **deadlock**. Es una herramienta educativa para entender cómo múltiples hilos pueden bloquearse mutuamente al acceder a recursos compartidos en un orden conflictivo.

---

## Descripción general

Un **deadlock** ocurre cuando dos o más hilos están esperando indefinidamente por recursos que nunca serán liberados porque otros hilos también están esperando, creando así una dependencia circular.

Este ejemplo simula un escenario clásico de interbloqueo utilizando dos `mutex` y dos hilos (`threadA` y `threadB`), donde cada hilo intenta adquirir los `mutex` en orden inverso.

---

##  Estructura del código

### Recursos y sincronización

```cpp
std::mutex mtx1, mtx2;
```


## `barrier.cpp` — Sincronización con `std::barrier`

Este archivo demuestra el uso de **barreras de sincronización** (`barriers`) en programación concurrente utilizando C++20.

## 📌 Descripción

Una **barrier** permite que un grupo de hilos (**threads**) se detenga en un punto de sincronización común hasta que todos los participantes hayan llegado. Luego, todos los hilos continúan su ejecución simultáneamente.

Esto es útil en situaciones donde se necesita garantizar que ciertos pasos de procesamiento no se inicien hasta que todos los hilos hayan completado una etapa previa.

### `std::barrier`

Una instancia de `std::barrier` se inicializa con el número total de hilos que deben sincronizarse. Cada hilo llama a `arrive_and_wait()` para indicar que ha llegado al punto de sincronización. El hilo quedará bloqueado hasta que el resto también haya llegado.

```cpp
#include <barrier>

std::barrier sync_point(5);  // Barrera para 5 hilos

void threadFunction() {
    // 🔄 Realiza tareas previas a la sincronización
    sync_point.arrive_and_wait();  // ⏸ Espera a que todos los hilos lleguen
    // ✅ Continúa después de la barrera
}
```



##  consideRendimiento.cpp — Productores y Consumidores con búfer compartido

Implementa un sistema multihilo basado en el patrón **Producer-Consumer**, utilizando primitivas modernas de sincronización de C++20 como `std::semaphore` y `std::mutex`.

El sistema consta de varios **productores** que generan tareas (ítems) y varios **consumidores** que las procesan. Ambos interactúan a través de un **búfer compartido** y sincronizado.

---

###  Componentes del sistema

**Constantes y variables globales:**

| Nombre             | Descripción                                                                            |
|--------------------|----------------------------------------------------------------------------------------|
| `BUFFER_SIZE`      | Tamaño máximo del búfer compartido.                                                   |
| `buffer`           | Cola (`std::queue`) que actúa como búfer para almacenar ítems producidos.             |
| `mtx`              | `std::mutex` para proteger el acceso concurrente al búfer.                            |
| `cv_producer` / `cv_consumer` | Variables de condición (`std::condition_variable`) para notificar entre hilos.        |
| `empty_slots`      | Semáforo que representa los espacios disponibles en el búfer.                         |
| `full_slots`       | Semáforo que representa los espacios ocupados en el búfer.                            |

---

###  Función del Productor

Cada hilo productor ejecuta el siguiente ciclo:

1. Produce `num_tasks` ítems.
2. Espera a que haya un espacio vacío disponible (`empty_slots.acquire()`).
3. Ingresa a la sección crítica protegida con `std::mutex`.
4. Inserta el ítem en el búfer y notifica a un consumidor (`cv_consumer.notify_one()`).
5. Libera un slot lleno (`full_slots.release()`).

---

###  Función del Consumidor

Cada hilo consumidor opera en bucle infinito:

1. Espera hasta que haya un ítem disponible (`full_slots.acquire()`).
2. Ingresa a la sección crítica y extrae un ítem del búfer.
3. Notifica a un productor (`cv_producer.notify_one()`).
4. Libera un slot vacío (`empty_slots.release()`).
5. Simula el procesamiento con un retardo.

---

###  Función `main()`

1. Crea múltiples hilos de productores y consumidores.
2. Lanza los hilos y espera a que los productores terminen (`join`).
3. Da tiempo a los consumidores para vaciar el búfer restante (`std::this_thread::sleep_for()`).
4. El programa termina forzadamente (en este caso, con un `terminate` no manejado).

---

### Ejemplo de salida en la terminal

```txt
Producer0 produced-item0
Consumer 1 consumed item0
Producer0 produced-item1
Producer1 produced-item100
Producer1 produced-item101
...
Consumer 2 consumed item9
Consumer 0 consumed item108
Consumer 1 consumed item109
All producers are finished
terminate called without an active exception
Aborted (core dumped)
```

* El sistema se comporta como un sistema multihilo convensional ya que en la salida los productores y consumidores operan de forma paralela y asincrónica. Al finalizar la ejecución, se confirma que todos los productores han completado su tarea correctamente.