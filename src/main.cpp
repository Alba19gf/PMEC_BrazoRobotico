#include <Arduino.h>
#include <stdio.h>
#include <limits.h>  // Para INT_MIN y INT_MAX

#define SIZE 3

// Declarar la matriz global
int MatrizEstado[SIZE][SIZE];

// Declarar la función externa
extern void Fn_MatrizState();

void convertir_matriz(int MatrizEstado[SIZE][SIZE], int MatrizEstadoNuevo[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (MatrizEstado[i][j] == 2)
                MatrizEstadoNuevo[i][j] = 0;  // Espacios vacíos se convierten a 0
            else if (MatrizEstado[i][j] == 0)
                MatrizEstadoNuevo[i][j] = -1;  // Cambia de representación 0 a -1
            else
                MatrizEstadoNuevo[i][j] = 1;  // Los demás valores se mantienen como 1
        }
    }
}

int comprueba_ganador(int MatrizEstadoNuevo[SIZE][SIZE], int *empate) {
    int combinaciones[8][3][2] = {
        {{0, 0}, {0, 1}, {0, 2}}, {{1, 0}, {1, 1}, {1, 2}}, {{2, 0}, {2, 1}, {2, 2}}, // filas
        {{0, 0}, {1, 0}, {2, 0}}, {{0, 1}, {1, 1}, {2, 1}}, {{0, 2}, {1, 2}, {2, 2}}, // columnas
        {{0, 0}, {1, 1}, {2, 2}}, {{0, 2}, {1, 1}, {2, 0}}  // diagonales
    };

    *empate = 1;  // Asume empate inicialmente
    for (int i = 0; i < 8; i++) {
        int a = combinaciones[i][0][0], b = combinaciones[i][0][1];
        int c = combinaciones[i][1][0], d = combinaciones[i][1][1];
        int e = combinaciones[i][2][0], f = combinaciones[i][2][1];

        if (MatrizEstadoNuevo[a][b] != 0 && MatrizEstadoNuevo[a][b] == MatrizEstadoNuevo[c][d] && MatrizEstadoNuevo[a][b] == MatrizEstadoNuevo[e][f]) {
            return MatrizEstadoNuevo[a][b];
        }
    }

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (MatrizEstadoNuevo[i][j] == 0) {
                *empate = 0;  // No es empate si hay un espacio vacío
                return 0;
            }
        }
    }

    return 0;  // Si no hay ganador y no hay espacios vacíos, es empate
}

int minimax(int MatrizEstadoNuevo[SIZE][SIZE], int prof, int isMax) {
    int empate;
    int ganador = comprueba_ganador(MatrizEstadoNuevo, &empate);

    if (ganador != 0) {
        return (ganador == 1 ? 100 - prof : -100 + prof);
    } else if (empate) {
        return 0;
    }

    if (isMax) {
        int mejorPuntuacion = INT_MIN;
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (MatrizEstadoNuevo[i][j] == 0) {
                    MatrizEstadoNuevo[i][j] = 1;
                    int puntuacion = minimax(MatrizEstadoNuevo, prof + 1, 0);
                    MatrizEstadoNuevo[i][j] = 0;
                    if (puntuacion > mejorPuntuacion) {
                        mejorPuntuacion = puntuacion;
                    }
                }
            }
        }
        return mejorPuntuacion;
    } else {
        int mejorPuntuacion = INT_MAX;
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (MatrizEstadoNuevo[i][j] == 0) {
                    MatrizEstadoNuevo[i][j] = -1;
                    int puntuacion = minimax(MatrizEstadoNuevo, prof + 1, 1);
                    MatrizEstadoNuevo[i][j] = 0;
                    if (puntuacion < mejorPuntuacion) {
                        mejorPuntuacion = puntuacion;
                    }
                }
            }
        }
        return mejorPuntuacion;
    }
}

void mov_optimo(int MatrizEstadoNuevo[SIZE][SIZE], int *fila, int *columna) {
    int mejor_puntuacion = INT_MIN;

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (MatrizEstadoNuevo[i][j] == 0) {
                MatrizEstadoNuevo[i][j] = 1;
                int puntuacion = minimax(MatrizEstadoNuevo, 0, 0);
                MatrizEstadoNuevo[i][j] = 0;

                if (puntuacion > mejor_puntuacion) {
                    mejor_puntuacion = puntuacion;
                    *fila = i;
                    *columna = j;
                }
            }
        }
    }
}

void setup() {
    Serial.begin(115200);

    // Llamar a la función externa para actualizar MatrizEstado
    Fn_MatrizState();

    int MatrizEstadoNuevo[SIZE][SIZE];
    convertir_matriz(MatrizEstado, MatrizEstadoNuevo);

    int empate;
    int ganador = comprueba_ganador(MatrizEstadoNuevo, &empate);

    if (ganador == 1) {
        Serial.println("El robot ha ganado.");
    } else if (ganador == -1) {
        Serial.println("El jugador humano ha ganado.");
    } else if (empate) {
        Serial.println("El juego ha terminado en empate.");
    } else {
        int fila, columna;
        mov_optimo(MatrizEstadoNuevo, &fila, &columna);
        MatrizEstadoNuevo[fila][columna] = 1;

        ganador = comprueba_ganador(MatrizEstadoNuevo, &empate);

        Serial.print("La máquina debe hacer su próximo movimiento en la fila ");
        Serial.print(fila);
        Serial.print(" y columna ");
        Serial.println(columna);

        if (ganador == 1) {
            Serial.println("El robot ha ganado con este movimiento.");
        } else if (ganador == -1) {
            Serial.println("El jugador humano ha ganado.");
        } else if (empate) {
            Serial.println("El juego ha terminado en empate con este movimiento.");
        }
    }
}

void loop() {
    // Nada que hacer aquí
}
