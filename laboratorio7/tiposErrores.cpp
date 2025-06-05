/**
 * @file tiposErrores.cpp
 * @author Diego Alfaro (diego.alfarosegura@ucr.ac.cr)
 * @brief Ejemplos de tipos de errores.
 * @version 0.1
 * @date 2025-06-04
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include <iostream>

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

    std::cout << "Suma de los elementos del arreglo: " << suma << std::endl;

    return 0;
}