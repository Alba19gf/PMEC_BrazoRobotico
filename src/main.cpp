#include <Arduino.h>
#include <stdio.h>

#define SIZE 3

void convertir_tablero(int tablero_original[SIZE][SIZE], int tablero_convertido[SIZE][SIZE])
 {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (tablero_original[i][j] == 2)
                tablero_convertido[i][j] = 0;
            else if (tablero_original[i][j] == 0)
                tablero_convertido[i][j] = -1;
            else
                tablero_convertido[i][j] = 1;
        }
    }
}

int comprueba_ganador(int tablero[SIZE][SIZE], int *empate)
 {
    int combinaciones[8][3][2] = {
        {{0, 0}, {0, 1}, {0, 2}}, {{1, 0}, {1, 1}, {1, 2}}, {{2, 0}, {2, 1}, {2, 2}}, // filas
        {{0, 0}, {1, 0}, {2, 0}}, {{0, 1}, {1, 1}, {2, 1}}, {{0, 2}, {1, 2}, {2, 2}}, // columnas
        {{0, 0}, {1, 1}, {2, 2}}, {{0, 2}, {1, 1}, {2, 0}}  // diagonales
    };

    *empate = 1;
    for (int i = 0; i < 8; i++) {
        int a = combinaciones[i][0][0], b = combinaciones[i][0][1];
        int c = combinaciones[i][1][0], d = combinaciones[i][1][1];
        int e = combinaciones[i][2][0], f = combinaciones[i][2][1];

        if (tablero[a][b] != 0 && tablero[a][b] == tablero[c][d] && tablero[a][b] == tablero[e][f]) {
            return tablero[a][b];
        }
    }

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (tablero[i][j] == 0) {
                *empate = 0;
                return 0;
            }
        }
    }

    return 0;
}

int minimax(int tablero[SIZE][SIZE], int prof, int isMax) 
{
    int empate;
    int ganador = comprueba_ganador(tablero, &empate);

    if (ganador != 0) {
        return (ganador == 1 ? 100 - prof : -100 + prof);
    } else if (empate) {
        return 0;
    }

    if (isMax) {
        int mejorPuntuacion = INT_MIN;
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (tablero[i][j] == 0) {
                    tablero[i][j] = 1;
                    int puntuacion = minimax(tablero, prof + 1, 0);
                    tablero[i][j] = 0;
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
                if (tablero[i][j] == 0) {
                    tablero[i][j] = -1;
                    int puntuacion = minimax(tablero, prof + 1, 1);
                    tablero[i][j] = 0;
                    if (puntuacion < mejorPuntuacion) {
                        mejorPuntuacion = puntuacion;
                    }
                }
            }
        }
        return mejorPuntuacion;
    }
}

void mov_optimo(int tablero_convertido[SIZE][SIZE], int *fila, int *columna) 
{
    int mejor_puntuacion = INT_MIN;
    int casilla_optima = -1;

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (tablero_convertido[i][j] == 0) {
                tablero_convertido[i][j] = 1;
                int puntuacion = minimax(tablero_convertido, 0, 0);
                tablero_convertido[i][j] = 0;

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

    int tablero_original[SIZE][SIZE] = {
        {-1, 2, -1},
        {2, 1, 2},
        {2, 1, 2}
    };

    int tablero_convertido[SIZE][SIZE];
    convertir_tablero(tablero_original, tablero_convertido);

    int empate;
    int ganador = comprueba_ganador(tablero_convertido, &empate);

    if (ganador == 1) {
        Serial.println("El robot ha ganado.");
    } else if (ganador == -1) {
        Serial.println("El jugador humano ha ganado.");
    } else if (empate) {
        Serial.println("El juego ha terminado en empate.");
    } else {
        int fila, columna;
        mov_optimo(tablero_convertido, &fila, &columna);
        tablero_convertido[fila][columna] = 1;

        ganador = comprueba_ganador(tablero_convertido, &empate);

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
