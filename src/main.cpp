#include <stdio.h>
#include <stdbool.h>

// Definimos constantes para representar el estado de cada celda del tablero
#define EMPTY 0
#define PLAYER_X 1
#define PLAYER_O 2

// Función para imprimir el tablero en la consola
void printBoard(int board[3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == PLAYER_X) {
                printf(" X "); // Representa el jugador X
            } else if (board[i][j] == PLAYER_O) {
                printf(" O "); // Representa el jugador O
            } else {
                printf(" . "); // Representa una celda vacía
            }
        }
        printf("\n");
    }
}

// Función para verificar si hay un ganador en el tablero
int checkWinner(int board[3][3]) {
    // Definimos las condiciones de victoria posibles
    int winConditions[8][3][2] = {
        {{0, 0}, {0, 1}, {0, 2}}, {{1, 0}, {1, 1}, {1, 2}}, {{2, 0}, {2, 1}, {2, 2}}, // Filas
        {{0, 0}, {1, 0}, {2, 0}}, {{0, 1}, {1, 1}, {2, 1}}, {{0, 2}, {1, 2}, {2, 2}}, // Columnas
        {{0, 0}, {1, 1}, {2, 2}}, {{0, 2}, {1, 1}, {2, 0}}  // Diagonales
    };

    // Verificamos si alguna condición de victoria se cumple
    for (int i = 0; i < 8; i++) {
        if (board[winConditions[i][0][0]][winConditions[i][0][1]] != EMPTY &&
            board[winConditions[i][0][0]][winConditions[i][0][1]] == board[winConditions[i][1][0]][winConditions[i][1][1]] &&
            board[winConditions[i][0][0]][winConditions[i][0][1]] == board[winConditions[i][2][0]][winConditions[i][2][1]]) {
            return board[winConditions[i][0][0]][winConditions[i][0][1]];
        }
    }

    // Verificamos si hay celdas vacías para determinar si el juego continúa
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == EMPTY) {
                return -1; // El juego continúa
            }
        }
    }
    return 0; // Empate
}

// Función Minimax para evaluar el mejor movimiento
int minimax(int board[3][3], int depth, bool isMaximizing) {
    // Evaluamos si hay un ganador o empate
    int score = checkWinner(board);
    if (score == PLAYER_X) return -10 + depth; // Jugador X gana
    if (score == PLAYER_O) return 10 - depth;  // Jugador O gana
    if (score == 0) return 0; // Empate

    if (isMaximizing) {
        int bestScore = -1000;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == EMPTY) {
                    board[i][j] = PLAYER_O; // Intentamos el movimiento
                    int value = minimax(board, depth + 1, false);
                    board[i][j] = EMPTY; // Deshacemos el movimiento
                    bestScore = (value > bestScore) ? value : bestScore;
                }
            }
        }
        return bestScore;
    } else {
        int bestScore = 1000;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == EMPTY) {
                    board[i][j] = PLAYER_X; // Intentamos el movimiento
                    int value = minimax(board, depth + 1, true);
                    board[i][j] = EMPTY; // Deshacemos el movimiento
                    bestScore = (value < bestScore) ? value : bestScore;
                }
            }
        }
        return bestScore;
    }
}

// Función para encontrar el mejor movimiento para la IA
void findBestMove(int board[3][3], int *bestMoveX, int *bestMoveY) {
    int bestScore = -1000;
    *bestMoveX = -1;
    *bestMoveY = -1;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == EMPTY) {
                board[i][j] = PLAYER_O; // Intentamos el movimiento
                int moveScore = minimax(board, 0, false);
                board[i][j] = EMPTY; // Deshacemos el movimiento
                if (moveScore > bestScore) {
                    bestScore = moveScore;
                    *bestMoveX = i;
                    *bestMoveY = j;
                }
            }
        }
    }
}

int main() {
    // Inicializamos el tablero vacío
    int board[3][3] = {
        {EMPTY, EMPTY, EMPTY},
        {EMPTY, EMPTY, EMPTY},
        {EMPTY, EMPTY, EMPTY}
    };

    int player = PLAYER_X; // Comienza el jugador X
    int bestMoveX, bestMoveY; // Variables para almacenar el mejor movimiento
    int winner;

    // Bucle principal del juego
    while ((winner = checkWinner(board)) == -1) {
        if (player == PLAYER_X) {
            int x, y;
            printf("Introduce la fila (0-2): ");
            scanf("%d", &x);
            printf("Introduce la columna (0-2): ");
            scanf("%d", &y);
            if (board[x][y] == EMPTY) {
                board[x][y] = PLAYER_X;
                player = PLAYER_O;
            } else {
                printf("Movimiento no válido. Inténtalo de nuevo.\n");
            }
        } else {
            findBestMove(board, &bestMoveX, &bestMoveY);
            board[bestMoveX][bestMoveY] = PLAYER_O;
            player = PLAYER_X;
        }

        printBoard(board);
        printf("\n");
    }

    // Anunciamos el resultado del juego
    if (winner == PLAYER_X) {
        printf("¡Has ganado!\n");
    } else if (winner == PLAYER_O) {
        printf("¡La IA ha ganado!\n");
    } else {
        printf("¡Empate!\n");
    }

    return 0;
}
