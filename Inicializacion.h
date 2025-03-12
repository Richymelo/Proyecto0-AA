// Inicializacion.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

int cuadro[21][21];


void inicializarCuadro(int orden) {
    for (int i = 0; i < orden; i++)
        for (int j = 0; j < orden; j++)
            cuadro[i][j] = 0;
}

int validarOrden(int orden) {
    return (orden >= 3 && orden <= 21 && orden % 2 == 1);
}

void procesarCuadroMagico(int matriz[21][21]) {
    int orden;

    printf("Ingrese el orden del cuadro mágico (impar entre 3 y 21): ");
    scanf("%d", &orden);

    if (!validarOrden(orden)) {
        printf("Orden inválido.\n");
        return;
    }

    printf("Orden válido. Puedes proceder con la generación del cuadro mágico.\n");
    // Aquí podrías llamar a una función para inicializar la matriz o llenarla
}

int main()
{
    std::cout << "Hello World!\n";
}
