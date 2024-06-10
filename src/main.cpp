//* ALGORITMO DE JUEGO AUTOMÁTICO
//Este código implementa un juego de tres en raya (tic-tac-toe) en el que el robot calcula su mejor movimiento usando el algoritmo Minimax y realiza su jugada. 
//La función Fn_MatrizState actualiza el estado del tablero desde una fuente externa, y el programa convierte ese estado a una representación interna para procesar el juego.

#include <Arduino.h>
#include <stdio.h>
#include <limits.h>  // Para INT_MIN y INT_MAX

#define SIZE 3 // Definir tablero de 3x3 

// Declarar la matriz global
int MatrizEstado[SIZE][SIZE];

// Declarar la función externa para recibir la matriz de visión
extern void Fn_MatrizState();


// void Fn_MatrizState() {
//     // Actualiza MatrizEstado con un estado de prueba
//     int estado_prueba[SIZE][SIZE] = {
//         {-1, 2, -1},
//         {2, 1, 2},
//         {2, 1, 2}
//     };
//     memcpy(MatrizEstado, estado_prueba, sizeof(estado_prueba));
// }


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

int comprueba_ganador(int MatrizEstadoNuevo[SIZE][SIZE], int *empate) {
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
    // Si el robot ha ganado 1
    // Si el humano ha ganado -1
}
//Algoritmo minimax explora rodas las posibles jugadas futuras y elegir la que maximiza la puntuación del robot
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

// La función mov_optimo utiliza el algoritmo minimax para encontar la mejor jugada posible para el robot
// Actualiza la fila y columna con las coordenadas de la mejor jugada
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

    // Convertir MatrizEstado a MatrizEstadoNuevo
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
    } else { // Si no hay ganador se encuentra el mejor movimiento para el robot
        int fila, columna;
        mov_optimo(MatrizEstadoNuevo, &fila, &columna);
        MatrizEstadoNuevo[fila][columna] = 1;

        // Se vuelve a comprobar el ganador
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
