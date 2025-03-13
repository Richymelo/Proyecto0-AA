/*
                Esta sección contiene las funciones de los movimientos
                en las cuales se pueden colocar los números. Está el método
                Siames, el movimiento en L, la espiral y el método alterno
                al Siames.
*/

#define max 21  // Tamaño máximo para la matriz

// Método Siamés clásico: mueve en diagonal arriba-derecha y ajusta si está ocupado
int metodoSiames(int matriz[max][max], int n, int fila, int columna) {
    int nuevaFila = (fila - 1 + n) % n;
    int nuevaColumna = (columna + 1) % n;

    if (matriz[nuevaFila][nuevaColumna] != 0) { 
        nuevaFila = (fila + 1) % n;
        nuevaColumna = columna;
    }

    return nuevaFila * max + nuevaColumna;
}

// Método en L: movimiento en forma de caballo del ajedrez
int metodoEnL(int matriz[max][max], int n, int fila, int columna) {
    int nuevaFila = (fila - 2 + n) % n;
    int nuevaColumna = (columna + 1) % n;
    if (matriz[nuevaFila][nuevaColumna] != 0) {
        nuevaFila = (fila + 1) % n;
        nuevaColumna = columna;
    }
    return nuevaFila * max + nuevaColumna;
}

// Método en Espiral: rellena en sentido de espiral alrededor del centro
int metodoEspiral(int matriz[max][max], int n, int fila, int columna) {
    int direcciones[4][2] = {{0,1}, {1,0}, {0,-1}, {-1,0}};
    static int dir = 0;
    int nuevaFila = fila + direcciones[dir][0];
    int nuevaColumna = columna + direcciones[dir][1];
    if (nuevaFila < 0 || nuevaFila >= n || nuevaColumna < 0 || nuevaColumna >= n || matriz[nuevaFila][nuevaColumna] != 0) {
        dir = (dir + 1) % 4;
        nuevaFila = fila + direcciones[dir][0];
        nuevaColumna = columna + direcciones[dir][1];
    }
    return nuevaFila * max + nuevaColumna;
}

// Método Alterno de Diagonales: variante del método siamés
int metodoAlterno(int matriz[max][max], int n, int fila, int columna) {
    int nuevaFila = (fila - 1 + n) % n;
    int nuevaColumna = (columna - 1 + n) % n;
    if (matriz[nuevaFila][nuevaColumna] != 0) {
        nuevaFila = (fila + 1) % n;
        nuevaColumna = columna;
    }
    return nuevaFila * max + nuevaColumna;
}