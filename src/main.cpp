//* ALGORITMO DE JUEGO AUTOMÁTICO
// Este código implementa un juego de tres en raya (tic-tac-toe) en el que el robot calcula su mejor movimiento usando el algoritmo Minimax y realiza su jugada.
// La función Fn_MatrizState actualiza el estado del tablero desde una fuente externa, y el programa convierte ese estado a una representación interna para procesar el juego.
#include <Arduino.h>
#include <stdio.h>
#include <limits.h> // Para INT_MIN y INT_MAX
#include <stdbool.h>

// Definición del tamaño del tablero
#define N 3

// Función para comprobar el ganador y si hay empate
void compruebaGanador(int MatrizEstadoNuevo[N][N], int *ganador, bool *empate) {
    int combinaciones[8][3] = {
        {0, 1, 2}, {3, 4, 5}, {6, 7, 8},  // filas
        {0, 3, 6}, {1, 4, 7}, {2, 5, 8},  // columnas
        {0, 4, 8}, {2, 4, 6}              // diagonales
    };

    *ganador = 0;
    *empate = true;

    for (int i = 0; i < 8; ++i) {
        int a = combinaciones[i][0];
        int b = combinaciones[i][1];
        int c = combinaciones[i][2];

        if (MatrizEstadoNuevo[a / N][a % N] == MatrizEstadoNuevo[b / N][b % N] &&
            MatrizEstadoNuevo[b / N][b % N] == MatrizEstadoNuevo[c / N][c % N] &&
            MatrizEstadoNuevo[a / N][a % N] != 0) {

            *ganador = MatrizEstadoNuevo[a / N][a % N];
            *empate = false;
            return;
        }

        if (MatrizEstadoNuevo[a / N][a % N] == 0 || MatrizEstadoNuevo[b / N][b % N] == 0 ||
            MatrizEstadoNuevo[c / N][c % N] == 0) {
            *empate = false;
        }
    }
}

// Función minimax recursiva
int minimax(int MatrizEstadoNuevo[N][N], int prof, bool isMax) {
    int ganador;
    bool empate;
    compruebaGanador(MatrizEstadoNuevo, &ganador, &empate);

    if (ganador == 1) {
        return 100 - prof; // Robot gana
    } else if (ganador == -1) {
        return -100 + prof; // Jugador humano gana
    } else if (empate) {
        return 0; // Empate
    }

    int mejorPuntuacion = isMax ? -1000 : 1000;

    for (int i = 0; i < N * N; ++i) {
        int row = i / N;
        int col = i % N;

        if (MatrizEstadoNuevo[row][col] == 0) {
            MatrizEstadoNuevo[row][col] = isMax ? 1 : -1;
            int puntuacion = minimax(MatrizEstadoNuevo, prof + 1, !isMax);
            MatrizEstadoNuevo[row][col] = 0;

            if (isMax && puntuacion > mejorPuntuacion) {
                mejorPuntuacion = puntuacion;
            } else if (!isMax && puntuacion < mejorPuntuacion) {
                mejorPuntuacion = puntuacion;
            }
        }
    }

    return mejorPuntuacion;
}

// Función para encontrar la mejor posición y resultado
void movOptimo(int MatrizEstadoNuevo[N][N], int *pos_optima, int *resultado) {
    int casillas_disponibles[N * N];
    int num_disponibles = 0;

    for (int i = 0; i < N * N; ++i) {
        int row = i / N;
        int col = i % N;

        if (MatrizEstadoNuevo[row][col] == 0) {
            casillas_disponibles[num_disponibles++] = i;
        }
    }

    int mejor_puntuacion = -1000;
    *pos_optima = -1;

    for (int i = 0; i < num_disponibles; ++i) {
        int row = casillas_disponibles[i] / N;
        int col = casillas_disponibles[i] % N;

        MatrizEstadoNuevo[row][col] = 1;
        int puntuacion = minimax(MatrizEstadoNuevo, 0, false);
        MatrizEstadoNuevo[row][col] = 0;

        if (puntuacion > mejor_puntuacion) {
            mejor_puntuacion = puntuacion;
            *pos_optima = casillas_disponibles[i];
        }
    }

    MatrizEstadoNuevo[*pos_optima / N][*pos_optima % N] = 1;
    int ganador;
    bool empate;
    compruebaGanador(MatrizEstadoNuevo, &ganador, &empate);

    if (ganador == 1) {
        *resultado = 11; // Robot ha ganado
    } else if (ganador == -1) {
        *resultado = 12; // Humano ha ganado
    } else if (empate) {
        *resultado = 13; // Empate
    } else {
        *resultado = 0; // No hay resultado definitivo
    }
}

// Función para convertir el tablero
void convertir_tablero(int MatrizEstado[N][N], int MatrizEstadoNuevo[N][N]) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (MatrizEstado[i][j] == 2) {
                MatrizEstadoNuevo[i][j] = 0; // Casilla vacía
            } else if (MatrizEstado[i][j] == 0) {
                MatrizEstadoNuevo[i][j] = -1; // Casilla con O (jugador humano)
            } else if (MatrizEstado[i][j] == 1) {
                MatrizEstadoNuevo[i][j] = 1; // Casilla con X (robot)
            }
        }
    }
}

int main() {
    int MatrizEstado[N][N] = {
        {1, 2, 2},
        {0, 1, 2},
        {0, 2, 2}
    };

    int MatrizEstadoNuevo[N][N];
    convertir_tablero(MatrizEstado, MatrizEstadoNuevo);

    // Mostrar el tablero convertido
    printf("MatrizEstadoNuevo:\n");
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            printf("%d ", MatrizEstadoNuevo[i][j]);
        }
        printf("\n");
    }

    int pos_optima, resultado;
    movOptimo(MatrizEstadoNuevo, &pos_optima, &resultado);

    // Mostrar el resultado
    if (resultado == 11) {
        printf("El robot ha ganado.\n");
    } else if (resultado == 12) {
        printf("El jugador humano ha ganado.\n");
    } else if (resultado == 13) {
        printf("El juego ha terminado en empate.\n");
    } else {
        printf("La máquina debe hacer su próximo movimiento en la casilla %d.\n", pos_optima + 1);
    }

    return 0;
}
