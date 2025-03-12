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

    printf("Ingrese el orden del cuadro m�gico (impar entre 3 y 21): ");
    scanf("%d", &orden);

    if (!validarOrden(orden)) {
        printf("Orden inv�lido.\n");
        return;
    }

    printf("Orden v�lido. Puedes proceder con la generaci�n del cuadro m�gico.\n");
    // Aqu� podr�as llamar a una funci�n para inicializar la matriz o llenarla
}

int main()
{
    std::cout << "Hello World!\n";
}
