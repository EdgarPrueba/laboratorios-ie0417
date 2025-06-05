# Universidad de Costa Rica  
**Escuela de Ingeniería Eléctrica**  
**IE-0417 – Diseño de Software para Ingeniería**  
**Laboratorio 7: Depuración, gdb, valgrind**  
**Integrantes:**  
- Diego Alfaro (C20259)  
- Edgar Alvarado (C10351)  
- Jean Zuñiga (C18767)  
---

## Concepto general: Depuración de errores 🔍

El laboratorio consiste en estudiar el proceso de identificar y eliminar errores o fallos en un programa de software, llamado depuración de errores. Se usan las siguientes herramientas para realizar la depuración, y se muestran ejemplos de su ejecución en distintas situaciones:

- [gdb (GNU Debugger)](#gdb-gnu-debugger)
- [Valgrind](#valgrind)
- [AddressSanitizer (ASan)](#addresssanitizer-asan)
- [ThreadSanitizer (TSan)](#threadsanitizer-tsan)
- [Helgrind](#helgrind)

### Tipos de errores 📝

Se tienen 3 tipos de errores principales, mostrados en el código. Estos generan errores en compilación y runtime, muestran las distintas formas en las que puede fallar un script. Compilar este archivo muestra el error de sintaxis y no genera un ejecutable, y si se corrige este error se genera el de división por 0, etc.

```c++
int main(){
    // Error de sintaxis: falta un punto y coma
    // Genera un error en compile time.
    std::cout << "Iniciando el programa" << std::endl

    // Error de tiempo de ejecución: división por cero
    // Genera un error en runtime.
    int divisor = 0;
    int resultado = 10 / divisor;

    // Error lógico: la condición del bucle debería ser i < 5
    // Podría generar un error en runtime.
    int arreglo[5] = {1, 2, 3, 4, 5};
    int suma = 0;
    for (int i = 0; i <= 5; i++) {
        suma += arreglo[i];
    }
}
```

### Breakpoints y stepping
Se utilizan "breakpoints" para marcar puntos de parada en un script y se usan "steps" para continuar la ejecución entre cada breakpoint de formas variadas. 

```c#
class Program
{
    static void Main(string[] args)
    {
        var objMath = new Math();   // Se coloca un breakpoint aquí
        // Ejecución se pausa, se puede continuar con steps.
        objMath.PrintSum(10, 20, 30, 40, 50);
    }
}

```

---

## Herramientas de depuración de errores 🖥️

### [gdb (GNU Debugger)](https://sourceware.org/gdb/documentation/)

Herramienta de depuración con diversos comandos para correr un script, crear breakpoints, mostrar estados, etc. Para un script con errores en runtime, se tienen los siguientes resultados:

```c++
#include <iostream>

void funcion1() {
    int a = 5;
    int b = 0;
    // Se agrega un breakpoint de prueba con "break"
    int c =  a / b;     //Error: División por cero, se mostrará como excepción SIGFPE.
    std::cout << "Resultado: " << c << std::endl;
}

int main() {
    std::cout << "Iniciando el programa" << std::endl;
    // Se coloca un breakpoint en la siguiente línea, con step no se pausa en el breakpoint superior.
    funcion1();
    std::cout << "Finalizando el programa" << std::endl;
    return 0;
}

```

Se compila con bandera del debugger "-g" (`g++ -g -o scriptGDB scriptGDB.cpp`) y se depura agregando un breakpoint y revisando variables que generan la excepción, además se muestra la pila de llamadas. Se nota que al crear el breakpoint, la ejecución del programa se detiene en ese punto. Además se recibe la lectura correcta del error de división por 0, y se verifica la fuente de error al mostrar el valor de b. Finalmente, la llamada de pilas muestra las funciones iniciadas correctamente.

```bash
gdb scriptGDB
(gdb) break scriptGDB.cpp:13
Breakpoint 1 at 0x125e: file scriptGDB.cpp, line 13.
(gdb) run
Starting program: /mnt/c/Users/daas0/Documents/Universidad/Semestre_7/Diseño/Labs/laboratorios-ie0417/laboratorio7/scriptGDB 
[Thread debugging using libthread_db enabled]
Using host libthread_db library "/lib/x86_64-linux-gnu/libthread_db.so.1".
Iniciando el programa

Breakpoint 1, main () at scriptGDB.cpp:13
13          funcion1();
(gdb) step
funcion1 () at scriptGDB.cpp:4
4           int a = 5;
(gdb)
5           int b = 0;
(gdb)
6           int c =  a / b;     //Error: División por cero, se mostrará como excepción SIGFPE.
(gdb)

Program received signal SIGFPE, Arithmetic exception.
0x00005555555551e7 in funcion1 () at scriptGDB.cpp:6
6           int c =  a / b;     //Error: División por cero, se mostrará como excepción SIGFPE.
(gdb) print b
$1 = 0
(gdb) backtrace
#0  0x00005555555551e7 in funcion1 () at scriptGDB.cpp:6
#1  0x0000555555555263 in main () at scriptGDB.cpp:13
(gdb) continue
Continuing.

Program terminated with signal SIGFPE, Arithmetic exception.
The program no longer exists.
```

---

### [Valgrind](https://valgrind.org/)

Herramienta para la detección de errores de manejo de memoria y concurrencia. Se tiene el siguiente script con errores de: asignación de memoria no liberada, uso de variables no inicializadas y acceso de memoria fuera de límites. Su principal herramienta es **Memcheck** Para depurar, primero se debe compilar el programa con la bandera -g (`g++ -g -o scriptMemcheck scriptMemcheck.cpp`) y ejecutar valgrind en el archivo (`valgrind --leak-check=yes scriptMemcheck`).

```c++
    int* q = new int[10];
    // No se libera la memoria asignada

    int* p = new int[10];
    std::cout << p[0];  // p[0] no está inicializado, pero se intenta usar
    delete p;
    
    int array[10];
    array[10] = 5;  // Acceso fuera de los límites del array
    return 0;
```

Se detectan los errores de uso de variables no inicializadas, la falta de uso de free para un new, etc. Se muestran los resultados de utilizar Valgrind en el script especificado. 

```bash
==6034== Memcheck, a memory error detector
==6034== Copyright (C) 2002-2017, and GNU GPL'd, by Julian Seward et al.
==6034== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==6034== Command: ./scriptMemcheck
==6034== 
==6034== Conditional jump or move depends on uninitialised value(s)
==6034==    at 0x4995A4E: std::ostreambuf_iterator<char, std::char_traits<char> > std::num_put<char, std::ostreambuf_iterator<char, std::char_traits<char> > >::_M_insert_int<long>(std::ostreambuf_iterator<char, std::char_traits<char> >, std::ios_base&, char, long) const (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.30)
==6034==    by 0x49A4119: std::ostream& std::ostream::_M_insert<long>(long) (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.30)
==6034==    by 0x109236: main (scriptMemcheck.cpp:8)
==6034==
==6034== Use of uninitialised value of size 8
==6034==    at 0x499592B: ??? (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.30)
==6034==    by 0x4995A78: std::ostreambuf_iterator<char, std::char_traits<char> > std::num_put<char, std::ostreambuf_iterator<char, std::char_traits<char> > >::_M_insert_int<long>(std::ostreambuf_iterator<char, std::char_traits<char> >, std::ios_base&, char, long) const (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.30)
==6034==    by 0x49A4119: std::ostream& std::ostream::_M_insert<long>(long) (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.30)
==6034==    by 0x109236: main (scriptMemcheck.cpp:8)
==6034==
==6034== Conditional jump or move depends on uninitialised value(s)
==6034==    at 0x499593D: ??? (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.30)
==6034==    by 0x4995A78: std::ostreambuf_iterator<char, std::char_traits<char> > std::num_put<char, std::ostreambuf_iterator<char, std::char_traits<char> > >::_M_insert_int<long>(std::ostreambuf_iterator<char, std::char_traits<char> >, std::ios_base&, char, long) const (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.30)
==6034==    by 0x49A4119: std::ostream& std::ostream::_M_insert<long>(long) (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.30)
==6034==    by 0x109236: main (scriptMemcheck.cpp:8)
==6034==
==6034== Conditional jump or move depends on uninitialised value(s)
==6034==    at 0x4995AAE: std::ostreambuf_iterator<char, std::char_traits<char> > std::num_put<char, std::ostreambuf_iterator<char, std::char_traits<char> > >::_M_insert_int<long>(std::ostreambuf_iterator<char, std::char_traits<char> >, std::ios_base&, char, long) const (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.30)
==6034==    by 0x49A4119: std::ostream& std::ostream::_M_insert<long>(long) (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.30)
==6034==    by 0x109236: main (scriptMemcheck.cpp:8)
==6034==
==6034== Mismatched free() / delete / delete []
==6034==    at 0x484BB6F: operator delete(void*, unsigned long) (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==6034==    by 0x10924C: main (scriptMemcheck.cpp:9)
==6034==  Address 0x4dd9cf0 is 0 bytes inside a block of size 40 alloc'd
==6034==    at 0x484A2F3: operator new[](unsigned long) (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==6034==    by 0x10921B: main (scriptMemcheck.cpp:7)
==6034==
*** stack smashing detected ***: terminated
==6034==
==6034== Process terminating with default action of signal 6 (SIGABRT)
==6034==    at 0x4B299FC: __pthread_kill_implementation (pthread_kill.c:44)
==6034==    by 0x4B299FC: __pthread_kill_internal (pthread_kill.c:78)
==6034==    by 0x4B299FC: pthread_kill@@GLIBC_2.34 (pthread_kill.c:89)
==6034==    by 0x4AD5475: raise (raise.c:26)
==6034==    by 0x4ABB7F2: abort (abort.c:79)
==6034==    by 0x4B1C676: __libc_message (libc_fatal.c:156)
==6034==    by 0x4BC9599: __fortify_fail (fortify_fail.c:26)
==6034==    by 0x4BC9565: __stack_chk_fail (stack_chk_fail.c:24)
==6034==    by 0x10926C: main (scriptMemcheck.cpp:14)
==6034==
==6034== HEAP SUMMARY:
==6034==     in use at exit: 73,768 bytes in 3 blocks
==6034==   total heap usage: 4 allocs, 1 frees, 73,808 bytes allocated
==6034==
==6034== LEAK SUMMARY:
==6034==    definitely lost: 0 bytes in 0 blocks
==6034==    indirectly lost: 0 bytes in 0 blocks
==6034==      possibly lost: 0 bytes in 0 blocks
==6034==    still reachable: 73,768 bytes in 3 blocks
==6034==         suppressed: 0 bytes in 0 blocks
==6034== Reachable blocks (those to which a pointer was found) are not shown.
==6034== To see them, rerun with: --leak-check=full --show-leak-kinds=all
==6034==
==6034== Use --track-origins=yes to see where uninitialised values come from
==6034== For lists of detected and suppressed errors, rerun with: -s
==6034== ERROR SUMMARY: 5 errors from 5 contexts (suppressed: 0 from 0)
Aborted (core dumped)

```

---

### [AddressSanitizer (ASan)](https://github.com/google/sanitizers/wiki/addresssanitizer)

Herramienta que detecta errores de memoria empleando una memoria de llamados/asignaciones incorrectas de memoria. Considerando el siguiente script con uso incorrecto de memoria dinámica:

```c++
void funcion1() {
    int* p = new int[10];
    p[10] = 5;  // Acceso fuera de los límites del array
    delete[] p;

    int* q = new int[10];
    delete[] q;
    std::cout << q[0] << std::endl; // Uso después de liberar memoria
}

int main() {
    funcion1();
    return 0;
}
```

Se compila con la bandera para el uso de sanitizers de direcciones de memoria (`g++ -fsanitize=address -g -o scriptASan scriptASan.cpp`) y se corre el archivo ejecutable resultante. Esto resulta en el reporte de memoria siguiente, el cual muestra un heap overflow por la escritura de una dirección fuera de la memoria creada y el uso de una dirección fuera de la región de bytes reservados (correspondiente al uso de memoria ya liberada).

```bash
=================================================================
==6119==ERROR: AddressSanitizer: heap-buffer-overflow on address 0x604000000038 at pc 0x5d67ed358342 bp 0x7fff493bc6c0 sp 0x7fff493bc6b0
WRITE of size 4 at 0x604000000038 thread T0
    #0 0x5d67ed358341 in funcion1() /mnt/c/Users/daas0/Documents/Universidad/Semestre_7/Diseño/Labs/laboratorios-ie0417/laboratorio7/scriptASan.cpp:5
    #1 0x5d67ed3583eb in main /mnt/c/Users/daas0/Documents/Universidad/Semestre_7/Diseño/Labs/laboratorios-ie0417/laboratorio7/scriptASan.cpp:14
    #2 0x768cb8829d8f in __libc_start_call_main ../sysdeps/nptl/libc_start_call_main.h:58
    #3 0x768cb8829e3f in __libc_start_main_impl ../csu/libc-start.c:392
    #4 0x5d67ed358224 in _start (/mnt/c/Users/daas0/Documents/Universidad/Semestre_7/Diseño/Labs/laboratorios-ie0417/laboratorio7/scriptASan+0x1224)

0x604000000038 is located 0 bytes to the right of 40-byte region [0x604000000010,0x604000000038)
allocated by thread T0 here:
    #0 0x768cb90b6357 in operator new[](unsigned long) ../../../../src/libsanitizer/asan/asan_new_delete.cpp:102
    #1 0x5d67ed3582fe in funcion1() /mnt/c/Users/daas0/Documents/Universidad/Semestre_7/Diseño/Labs/laboratorios-ie0417/laboratorio7/scriptASan.cpp:4
    #2 0x5d67ed3583eb in main /mnt/c/Users/daas0/Documents/Universidad/Semestre_7/Diseño/Labs/laboratorios-ie0417/laboratorio7/scriptASan.cpp:14
    #3 0x768cb8829d8f in __libc_start_call_main ../sysdeps/nptl/libc_start_call_main.h:58

SUMMARY: AddressSanitizer: heap-buffer-overflow /mnt/c/Users/daas0/Documents/Universidad/Semestre_7/Diseño/Labs/laboratorios-ie0417/laboratorio7/scriptASan.cpp:5 in funcion1()
Shadow bytes around the buggy address:
  0x0c087fff7fb0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x0c087fff7fc0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x0c087fff7fd0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x0c087fff7fe0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
  0x0c087fff7ff0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
=>0x0c087fff8000: fa fa 00 00 00 00 00[fa]fa fa fa fa fa fa fa fa
  0x0c087fff8010: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c087fff8020: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c087fff8030: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c087fff8040: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c087fff8050: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
Shadow byte legend (one shadow byte represents 8 application bytes):
  Addressable:           00
  Partially addressable: 01 02 03 04 05 06 07
  Heap left redzone:       fa
  Freed heap region:       fd
  Stack left redzone:      f1
  Stack mid redzone:       f2
  Stack right redzone:     f3
  Stack after return:      f5
  Stack use after scope:   f8
  Global redzone:          f9
  Global init order:       f6
  Poisoned by user:        f7
  Container overflow:      fc
  Array cookie:            ac
  Intra object redzone:    bb
  ASan internal:           fe
  Left alloca redzone:     ca
  Right alloca redzone:    cb
  Shadow gap:              cc
==6119==ABORTING
```

---

### [ThreadSanitizer (TSan)](https://clang.llvm.org/docs/ThreadSanitizer.html)

Similar al sanitizer de direcciones de memoria "ASan", el sanitizer de threads "TSan" permite detectar errores de concurrencia y memoria, pero en programas que emplean hilos de paralelismo. Particularmente se emplean para detectar condiciones de carrera. Con el siguiente script de una función multihilo que puede generar condiciones de carrera:

```c++
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
```

Se compila el programa con la bandera de sanitizer de threads (`g++ -fsanitize=thread -g -o scriptTSanHelgrind scriptTSanHelgrind.cpp`), y se corre el ejecutable resultante para mostrar los siguientes resultados. En el output del programa se señala un data race (condición de carrera) e indica que el thread T2 generó una lectura en la misma dirección de memoria en la que la thread T1 escribió previamente. **NOTA: En algunos casos se debe reducir la entropía del sistema ASLR para que no se genere un error con el sistema del TSan. Se usa el comando `sudo sysctl vm.mmap_rnd_bits=28` en el caso de WSL.**

```bash
==================
WARNING: ThreadSanitizer: data race (pid=6761)
  Read of size 4 at 0x559121d6a154 by thread T2:
    #0 increment() /mnt/c/Users/daas0/Documents/Universidad/Semestre_7/Diseño/Labs/laboratorios-ie0417/laboratorio7/scriptTSanHelgrind.cpp:10 (scriptTSanHelgrind+0x13f9)     
    #1 void std::__invoke_impl<void, void (*)()>(std::__invoke_other, void (*&&)()) /usr/include/c++/11/bits/invoke.h:61 (scriptTSanHelgrind+0x228a)
    #2 std::__invoke_result<void (*)()>::type std::__invoke<void (*)()>(void (*&&)()) /usr/include/c++/11/bits/invoke.h:96 (scriptTSanHelgrind+0x21df)
    #3 void std::thread::_Invoker<std::tuple<void (*)()> >::_M_invoke<0ul>(std::_Index_tuple<0ul>) /usr/include/c++/11/bits/std_thread.h:259 (scriptTSanHelgrind+0x2134)      
    #4 std::thread::_Invoker<std::tuple<void (*)()> >::operator()() /usr/include/c++/11/bits/std_thread.h:266 (scriptTSanHelgrind+0x20d6)
    #5 std::thread::_State_impl<std::thread::_Invoker<std::tuple<void (*)()> > >::_M_run() /usr/include/c++/11/bits/std_thread.h:211 (scriptTSanHelgrind+0x2088)
    #6 <null> <null> (libstdc++.so.6+0xdc252)

  Previous write of size 4 at 0x559121d6a154 by thread T1:
    #0 increment() /mnt/c/Users/daas0/Documents/Universidad/Semestre_7/Diseño/Labs/laboratorios-ie0417/laboratorio7/scriptTSanHelgrind.cpp:10 (scriptTSanHelgrind+0x1411)     
    #1 void std::__invoke_impl<void, void (*)()>(std::__invoke_other, void (*&&)()) /usr/include/c++/11/bits/invoke.h:61 (scriptTSanHelgrind+0x228a)
    #2 std::__invoke_result<void (*)()>::type std::__invoke<void (*)()>(void (*&&)()) /usr/include/c++/11/bits/invoke.h:96 (scriptTSanHelgrind+0x21df)
    #3 void std::thread::_Invoker<std::tuple<void (*)()> >::_M_invoke<0ul>(std::_Index_tuple<0ul>) /usr/include/c++/11/bits/std_thread.h:259 (scriptTSanHelgrind+0x2134)      
    #4 std::thread::_Invoker<std::tuple<void (*)()> >::operator()() /usr/include/c++/11/bits/std_thread.h:266 (scriptTSanHelgrind+0x20d6)
    #5 std::thread::_State_impl<std::thread::_Invoker<std::tuple<void (*)()> > >::_M_run() /usr/include/c++/11/bits/std_thread.h:211 (scriptTSanHelgrind+0x2088)
    #6 <null> <null> (libstdc++.so.6+0xdc252)

  Location is global 'counter' of size 4 at 0x559121d6a154 (scriptTSanHelgrind+0x000000005154)

  Thread T2 (tid=6764, running) created by main thread at:
    #0 pthread_create ../../../../src/libsanitizer/tsan/tsan_interceptors_posix.cpp:969 (libtsan.so.0+0x605b8)
    #1 std::thread::_M_start_thread(std::unique_ptr<std::thread::_State, std::default_delete<std::thread::_State> >, void (*)()) <null> (libstdc++.so.6+0xdc328)
    #2 main /mnt/c/Users/daas0/Documents/Universidad/Semestre_7/Diseño/Labs/laboratorios-ie0417/laboratorio7/scriptTSanHelgrind.cpp:18 (scriptTSanHelgrind+0x1484)

  Thread T1 (tid=6763, finished) created by main thread at:
    #0 pthread_create ../../../../src/libsanitizer/tsan/tsan_interceptors_posix.cpp:969 (libtsan.so.0+0x605b8)
    #1 std::thread::_M_start_thread(std::unique_ptr<std::thread::_State, std::default_delete<std::thread::_State> >, void (*)()) <null> (libstdc++.so.6+0xdc328)
    #2 main /mnt/c/Users/daas0/Documents/Universidad/Semestre_7/Diseño/Labs/laboratorios-ie0417/laboratorio7/scriptTSanHelgrind.cpp:17 (scriptTSanHelgrind+0x146e)

SUMMARY: ThreadSanitizer: data race /mnt/c/Users/daas0/Documents/Universidad/Semestre_7/Diseño/Labs/laboratorios-ie0417/laboratorio7/scriptTSanHelgrind.cpp:10 in increment() 
==================
Counter: 200000
ThreadSanitizer: reported 1 warnings
```

---

### [Helgrind](https://valgrind.org/docs/manual/hg-manual.html)

Herramienta incluida entre las implementadas por Valgrind, se utiliza para depurar errores de sistemas multihilo propensos a bloqueos, condiciones de carrera y demás. Facilitan la corrección de errores en sistemas difíciles de depurar. Se emplea el mismo script utilizado para la demostración de la herramienta TSan, pero empleando una configuración similar a la utilizada para Valgrind. Se compila el programa con la bandera para incluir información de depuración (`g++ -g -o scriptTsanHelgrind scriptTsanHelgrind.cpp`) y se utiliza la interfaz de Valgrind especificando la herramienta de Helgrind (`valgrind --tool=helgrind ./scriptTsanHelgrind`). Esto genera los resultados siguientes errores indicando posibles condiciones de carrera de forma cronológica (primero la creación de los threads y luego las condiciones de carrera). Se muestran diversas condiciones de carreras y el hecho que no se detectaron locks para evitar las condiciones de carrera.

```bash
==6877== Helgrind, a thread error detector
==6877== Copyright (C) 2007-2017, and GNU GPL'd, by OpenWorks LLP et al.
==6877== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==6877== Command: ./scriptTsanHelgrind
==6877== 
==6877== ---Thread-Announcement------------------------------------------
==6877== 
==6877== Thread #3 was created
==6877==    at 0x4BE09F3: clone (clone.S:76)
==6877==    by 0x4BE18EE: __clone_internal (clone-internal.c:83)
==6877==    by 0x4B4F6D8: create_thread (pthread_create.c:295)
==6877==    by 0x4B501FF: pthread_create@@GLIBC_2.34 (pthread_create.c:828)
==6877==    by 0x4853767: ??? (in /usr/libexec/valgrind/vgpreload_helgrind-amd64-linux.so)
==6877==    by 0x494B328: std::thread::_M_start_thread(std::unique_ptr<std::thread::_State, std::default_delete<std::thread::_State> >, void (*)()) (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.30)
==6877==    by 0x1095B7: std::thread::thread<void (&)(), , void>(void (&)()) (std_thread.h:143)
==6877==    by 0x109341: main (scriptTsanHelgrind.cpp:18)
==6877==
==6877== ---Thread-Announcement------------------------------------------
==6877==
==6877== Thread #2 was created
==6877==    at 0x4BE09F3: clone (clone.S:76)
==6877==    by 0x4BE18EE: __clone_internal (clone-internal.c:83)
==6877==    by 0x4B4F6D8: create_thread (pthread_create.c:295)
==6877==    by 0x4B501FF: pthread_create@@GLIBC_2.34 (pthread_create.c:828)
==6877==    by 0x4853767: ??? (in /usr/libexec/valgrind/vgpreload_helgrind-amd64-linux.so)
==6877==    by 0x494B328: std::thread::_M_start_thread(std::unique_ptr<std::thread::_State, std::default_delete<std::thread::_State> >, void (*)()) (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.30)
==6877==    by 0x1095B7: std::thread::thread<void (&)(), , void>(void (&)()) (std_thread.h:143)
==6877==    by 0x10932B: main (scriptTsanHelgrind.cpp:17)
==6877==
==6877== ----------------------------------------------------------------
==6877==
==6877== Possible data race during read of size 4 at 0x10C154 by thread #3
==6877== Locks held: none
==6877==    at 0x1092DA: increment() (scriptTsanHelgrind.cpp:10)
==6877==    by 0x109C1E: void std::__invoke_impl<void, void (*)()>(std::__invoke_other, void (*&&)()) (invoke.h:61)
==6877==    by 0x109BCA: std::__invoke_result<void (*)()>::type std::__invoke<void (*)()>(void (*&&)()) (invoke.h:96)
==6877==    by 0x109B6B: void std::thread::_Invoker<std::tuple<void (*)()> >::_M_invoke<0ul>(std::_Index_tuple<0ul>) (std_thread.h:259)
==6877==    by 0x109B3B: std::thread::_Invoker<std::tuple<void (*)()> >::operator()() (std_thread.h:266)
==6877==    by 0x109B1B: std::thread::_State_impl<std::thread::_Invoker<std::tuple<void (*)()> > >::_M_run() (std_thread.h:211)
==6877==    by 0x494B252: ??? (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.30)
==6877==    by 0x485396A: ??? (in /usr/libexec/valgrind/vgpreload_helgrind-amd64-linux.so)
==6877==    by 0x4B4FAC2: start_thread (pthread_create.c:442)
==6877==    by 0x4BE0A03: clone (clone.S:100)
==6877==
==6877== This conflicts with a previous write of size 4 by thread #2
==6877== Locks held: none
==6877==    at 0x1092E3: increment() (scriptTsanHelgrind.cpp:10)
==6877==    by 0x109C1E: void std::__invoke_impl<void, void (*)()>(std::__invoke_other, void (*&&)()) (invoke.h:61)
==6877==    by 0x109BCA: std::__invoke_result<void (*)()>::type std::__invoke<void (*)()>(void (*&&)()) (invoke.h:96)
==6877==    by 0x109B6B: void std::thread::_Invoker<std::tuple<void (*)()> >::_M_invoke<0ul>(std::_Index_tuple<0ul>) (std_thread.h:259)
==6877==    by 0x109B3B: std::thread::_Invoker<std::tuple<void (*)()> >::operator()() (std_thread.h:266)
==6877==    by 0x109B1B: std::thread::_State_impl<std::thread::_Invoker<std::tuple<void (*)()> > >::_M_run() (std_thread.h:211)
==6877==    by 0x494B252: ??? (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.30)
==6877==    by 0x485396A: ??? (in /usr/libexec/valgrind/vgpreload_helgrind-amd64-linux.so)
==6877==  Address 0x10c154 is 0 bytes inside data symbol "counter"
==6877==
==6877== ----------------------------------------------------------------
==6877==
==6877== Possible data race during write of size 4 at 0x10C154 by thread #3
==6877== Locks held: none
==6877==    at 0x1092E3: increment() (scriptTsanHelgrind.cpp:10)
==6877==    by 0x109C1E: void std::__invoke_impl<void, void (*)()>(std::__invoke_other, void (*&&)()) (invoke.h:61)
==6877==    by 0x109BCA: std::__invoke_result<void (*)()>::type std::__invoke<void (*)()>(void (*&&)()) (invoke.h:96)
==6877==    by 0x109B6B: void std::thread::_Invoker<std::tuple<void (*)()> >::_M_invoke<0ul>(std::_Index_tuple<0ul>) (std_thread.h:259)
==6877==    by 0x109B3B: std::thread::_Invoker<std::tuple<void (*)()> >::operator()() (std_thread.h:266)
==6877==    by 0x109B1B: std::thread::_State_impl<std::thread::_Invoker<std::tuple<void (*)()> > >::_M_run() (std_thread.h:211)
==6877==    by 0x494B252: ??? (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.30)
==6877==    by 0x485396A: ??? (in /usr/libexec/valgrind/vgpreload_helgrind-amd64-linux.so)
==6877==    by 0x4B4FAC2: start_thread (pthread_create.c:442)
==6877==    by 0x4BE0A03: clone (clone.S:100)
==6877==
==6877== This conflicts with a previous write of size 4 by thread #2
==6877== Locks held: none
==6877==    at 0x1092E3: increment() (scriptTsanHelgrind.cpp:10)
==6877==    by 0x109C1E: void std::__invoke_impl<void, void (*)()>(std::__invoke_other, void (*&&)()) (invoke.h:61)
==6877==    by 0x109BCA: std::__invoke_result<void (*)()>::type std::__invoke<void (*)()>(void (*&&)()) (invoke.h:96)
==6877==    by 0x109B6B: void std::thread::_Invoker<std::tuple<void (*)()> >::_M_invoke<0ul>(std::_Index_tuple<0ul>) (std_thread.h:259)
==6877==    by 0x109B3B: std::thread::_Invoker<std::tuple<void (*)()> >::operator()() (std_thread.h:266)
==6877==    by 0x109B1B: std::thread::_State_impl<std::thread::_Invoker<std::tuple<void (*)()> > >::_M_run() (std_thread.h:211)
==6877==    by 0x494B252: ??? (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.30)
==6877==    by 0x485396A: ??? (in /usr/libexec/valgrind/vgpreload_helgrind-amd64-linux.so)
==6877==  Address 0x10c154 is 0 bytes inside data symbol "counter"
==6877==
Counter: 200000
==6877== 
==6877== Use --history-level=approx or =none to gain increased speed, at
==6877== the cost of reduced accuracy of conflicting-access information
==6877== For lists of detected and suppressed errors, rerun with: -s
==6877== ERROR SUMMARY: 2 errors from 2 contexts (suppressed: 0 from 0)
```

Si se toman medidas para coordinar los cambios entre hilos correctamente, empleando un lock_guard, el mensaje resultante indica que no se encontraron errores de condiciones de carrera.

```bash
==6944== Helgrind, a thread error detector
==6944== Copyright (C) 2007-2017, and GNU GPL'd, by OpenWorks LLP et al.
==6944== Using Valgrind-3.18.1 and LibVEX; rerun with -h for copyright info
==6944== Command: ./scriptHelgrindCorregido
==6944==
Counter: 200000
==6944== 
==6944== Use --history-level=approx or =none to gain increased speed, at
==6944== the cost of reduced accuracy of conflicting-access information
==6944== For lists of detected and suppressed errors, rerun with: -s
==6944== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 116644 from 7)
```

---
