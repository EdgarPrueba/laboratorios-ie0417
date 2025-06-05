#include <iostream>

void funcion1() {
    int a = 5;
    int b = 0;
    int c =  a / b;     //Error: División por cero, se mostrará como excepción SIGFPE.
    std::cout << "Resultado: " << c << std::endl;
}

int main() {
    std::cout << "Iniciando el programa" << std::endl;
    // Se coloca un breakpoint en esta línea, con step sigue la depuración entrando a funcion1.
    funcion1();
    std::cout << "Finalizando el programa" << std::endl;
    return 0;
}