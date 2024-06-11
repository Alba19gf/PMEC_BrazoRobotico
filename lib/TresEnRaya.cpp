#include "TresEnRaya.h"

void Fn_MatrizState(int MatrizEstado[SIZE][SIZE]) {
    // Aquí se debe actualizar la matrizEstado desde la fuente externa
    // Ejemplo: MatrizEstado = actualizar_matriz(); // 
}

void convertir_matriz(int MatrizEstado[SIZE][SIZE], int MatrizEstadoNuevo[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (MatrizEstado[i][j] == 2)
                MatrizEstadoNuevo[i][j] = 0;  // CASILLAS VACÍAS se convierten a 0 
            else if (MatrizEstado[i][j] == 0)
                MatrizEstadoNuevo[i][j] = -1;  // Cambia de representación 0 a -1  JUGADOR HUMANO
            else
                MatrizEstadoNuevo[i][j] = 1;  // Los demás valores se mantienen como 1  JUGADOR ROBOT
        }
    }
}

void comprueba_ganador(int MatrizEstadoNuevo[SIZE][SIZE], int *empate) {
    int combinaciones[8][3][2] = {
        {{0, 0}, {0, 1}, {0, 2}}, {{1, 0}, {1, 1}, {1, 2}}, {{2, 0}, {2, 1}, {2, 2}}, // Combinaciones para filas
        {{0, 0}, {1, 0}, {2, 0}}, {{0, 1}, {1, 1}, {2, 1}}, {{0, 2}, {1, 2}, {2, 2}}, // Combinaciones para columnas
        {{0, 0}, {1, 1}, {2, 2}}, {{0, 2}, {1, 1}, {2, 0}}  // Combinaciones para diagonales
    };

    *empate = 1;  // Asume empate inicialmente
    for (int i = 0; i < 8; i++) {
        int a = combinaciones[i][0][0], b = combinaciones[i][0][1];
        int c = combinaciones[i][1][0], d = combinaciones[i][1][1];
        int e = combinaciones[i][2][0], f = combinaciones[i][2][1];

        if (MatrizEstadoNuevo[a][b] != 0 && MatrizEstadoNuevo[a][b] == MatrizEstadoNuevo[c][d] && MatrizEstadoNuevo[a][b] == MatrizEstadoNuevo[e][f]) {
            *empate = 0;
            return;
        }
    }

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (MatrizEstadoNuevo[i][j] == 0) {
                *empate = 0;  // No es empate si hay un espacio vacío
                return;
            }
        }
    }

    *empate = 1; // Si no hay ganador y no hay espacios vacíos, es empate
    // Si el robot ha ganado 1
    // Si el humano ha ganado -1
}

void minimax(int MatrizEstadoNuevo[SIZE][SIZE], int prof, int isMax, int* resultado) {
    int empate;
    comprueba_ganador(MatrizEstadoNuevo, &empate);

    if (empate) {
        *resultado = 0;
        return;
    }

    if (isMax) {
        int mejorPuntuacion = INT_MIN;
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (MatrizEstadoNuevo[i][j] == 0) {
                    MatrizEstadoNuevo[i][j] = 1;
                    minimax(MatrizEstadoNuevo, prof + 1, 0, resultado);
                    MatrizEstadoNuevo[i][j] = 0;
                    if (*resultado > mejorPuntuacion) {
                        mejorPuntuacion = *resultado;
                    }
                }
            }
        }
        *resultado = mejorPuntuacion;
    } else {
        int mejorPuntuacion = INT_MAX;
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (MatrizEstadoNuevo[i][j] == 0) {
                    MatrizEstadoNuevo[i][j] = -1;
                    minimax(MatrizEstadoNuevo, prof + 1, 1, resultado);
                    MatrizEstadoNuevo[i][j] = 0;
                    if (*resultado < mejorPuntuacion) {
                        mejorPuntuacion = *resultado;
                    }
                }
            }
        }
        *resultado = mejorPuntuacion;
    }
}

void mov_optimo(int MatrizEstado[SIZE][SIZE], int* pos_optima, int* resultado) {
    int MatrizEstadoNuevo[SIZE][SIZE];
    convertir_matriz(MatrizEstado, MatrizEstadoNuevo);

    int mejor_puntuacion = INT_MIN;
    int mejor_casilla = -1;

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (MatrizEstadoNuevo[i][j] == 0) {
                MatrizEstadoNuevo[i][j] = 1;
                minimax(MatrizEstadoNuevo, 0, 0, resultado);
                MatrizEstadoNuevo[i][j] = 0;

                if (*resultado > mejor_puntuacion) {
                    mejor_puntuacion = *resultado;
                    mejor_casilla = i * SIZE + j; // Calcular la posición óptima
                }
            }
        }
    }

    *pos_optima = mejor_casilla;
}

void ejecutarJuego() {
    // Llamar a la función externa para actualizar MatrizEstado
    int MatrizEstado[SIZE][SIZE];
    Fn_MatrizState(MatrizEstado);

    // Convertir MatrizEstado a MatrizEstadoNuevo
    int MatrizEstadoNuevo[SIZE][SIZE];
    convertir_matriz(MatrizEstado, MatrizEstadoNuevo);

    // Obtener la posición óptima
    int pos_optima = mov_optimo(MatrizEstadoNuevo);

    // Comprobar el resultado y asignar el valor adecuado
    int resultado = comprueba_ganador(MatrizEstadoNuevo);
    if (resultado == 1) {
        resultado = 13; // Empate
    } else if (resultado == 12) {
        // El robot ha ganado
    } else if (resultado == 11) {
        // El humano ha ganado
    }
}