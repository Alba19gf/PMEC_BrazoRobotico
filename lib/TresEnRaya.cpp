#include "TresEnRaya.h"

TresEnRaya::TresEnRaya() {
    SIZE = 3;
    MatrizEstado = new int*[SIZE];
    for (int i = 0; i < SIZE; ++i) {
        MatrizEstado[i] = new int[SIZE];
    }
}

void TresEnRaya::inicializar() {
    // Llamar a la función externa para actualizar MatrizEstado
    Fn_MatrizState();

    // Convertir MatrizEstado a MatrizEstadoNuevo
    int MatrizEstadoNuevo[SIZE][SIZE];
    convertirMatriz(MatrizEstado, MatrizEstadoNuevo);

    // Obtener el resultado
    resultado = 0;
    int ganador = compruebaGanador(MatrizEstadoNuevo, &resultado);
    if (ganador == 1) {
        resultado = 12; // El robot ha ganado
    } else if (ganador == -1) {
        resultado = 11; // El humano ha ganado
    } else if (resultado == 1) {
        resultado = 13; // Empate
    }
}

int TresEnRaya::obtenerPosicionOptima() {
    // Convertir MatrizEstado a MatrizEstadoNuevo
    int MatrizEstadoNuevo[SIZE][SIZE];
    convertirMatriz(MatrizEstado, MatrizEstadoNuevo);

    // Obtener la posición óptima
    return movOptimo(MatrizEstadoNuevo);
}

int TresEnRaya::obtenerResultado() {
    return resultado;
}

void TresEnRaya::convertirMatriz(int MatrizEstado[SIZE][SIZE], int MatrizEstadoNuevo[SIZE][SIZE]) {
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

int TresEnRaya::compruebaGanador(int MatrizEstadoNuevo[SIZE][SIZE], int *empate) {
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
}

int TresEnRaya::minimax(int MatrizEstadoNuevo[SIZE][SIZE], int prof, int isMax) {
    int empate;
    int ganador = compruebaGanador(MatrizEstadoNuevo, &empate);

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
                if (MatrizEstadoNuevo[i][j] == 0)
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

int TresEnRaya::movOptimo(int MatrizEstadoNuevo[SIZE][SIZE]) {
    int mejor_puntuacion = INT_MIN;
    int mejor_casilla = -1;

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (MatrizEstadoNuevo[i][j] == 0) {
                MatrizEstadoNuevo[i][j] = 1;
                int puntuacion = minimax(MatrizEstadoNuevo, 0, 0);
                MatrizEstadoNuevo[i][j] = 0;

                if (puntuacion > mejor_puntuacion) {
                    mejor_puntuacion = puntuacion;
                    mejor_casilla = i * SIZE + j; // Calcular la posición única
                }
            }
        }
    }

    return mejor_casilla;
}

// void Fn_MatrizState() {
//     // Actualiza MatrizEstado con un estado de prueba
//     int estado_prueba[SIZE][SIZE] = {
//         {0, 2, 0},
//         {2, 1, 2},
//         {2, 1, 2}
//     };
//     memcpy(MatrizEstado, estado_prueba, sizeof(estado_prueba));
// }

void setup() {
    // Inicializar comunicación serial
    Serial.begin(115200);

    // Llamar a la función externa para actualizar MatrizEstado
    Fn_MatrizState();

    // Crear objeto de la clase TresEnRaya
    TresEnRaya juego;

    // Inicializar el juego
    juego.inicializar();

    // Obtener la posición óptima
    int pos_optima = juego.obtenerPosicionOptima();

    // Obtener el resultado del juego
    int resultado = juego.obtenerResultado();

    // Enviar resultados a través de Serial
    Serial.print("Posición óptima: ");
    Serial.println(pos_optima);
    Serial.print("Resultado: ");
    Serial.println(resultado);
}

void loop() {
    // Nothing to loop here
}
