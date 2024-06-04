#include <stdio.h>
#include <stdbool.h>

#define EMPTY 0
#define PLAYER_X 1
#define PLAYER_O 2

void printBoard(int board[3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == PLAYER_X) {
                printf(" X ");
            } else if (board[i][j] == PLAYER_O) {
                printf(" O ");
            } else {
                printf(" . ");
            }
        }
        printf("\n");
    }
}

int checkWinner(int board[3][3]) {
    int winConditions[8][3][2] = {
        {{0, 0}, {0, 1}, {0, 2}}, {{1, 0}, {1, 1}, {1, 2}}, {{2, 0}, {2, 1}, {2, 2}}, 
        {{0, 0}, {1, 0}, {2, 0}}, {{0, 1}, {1, 1}, {2, 1}}, {{0, 2}, {1, 2}, {2, 2}}, 
        {{0, 0}, {1, 1}, {2, 2}}, {{0, 2}, {1, 1}, {2, 0}}
    };

    for (int i = 0; i < 8; i++) {
        if (board[winConditions[i][0][0]][winConditions[i][0][1]] != EMPTY &&
            board[winConditions[i][0][0]][winConditions[i][0][1]] == board[winConditions[i][1][0]][winConditions[i][1][1]] &&
            board[winConditions[i][0][0]][winConditions[i][0][1]] == board[winConditions[i][2][0]][winConditions[i][2][1]]) {
            return board[winConditions[i][0][0]][winConditions[i][0][1]];
        }
    }

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == EMPTY) {
                return -1;
            }
        }
    }
    return 0;
}

int minimax(int board[3][3], int depth, bool isMaximizing) {
    int score = checkWinner(board);
    if (score == PLAYER_X) return -10 + depth;
    if (score == PLAYER_O) return 10 - depth;
    if (score == 0) return 0;

    if (isMaximizing) {
        int bestScore = -1000;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == EMPTY) {
                    board[i][j] = PLAYER_O;
                    int value = minimax(board, depth + 1, false);
                    board[i][j] = EMPTY;
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
                    board[i][j] = PLAYER_X;
                    int value = minimax(board, depth + 1, true);
                    board[i][j] = EMPTY;
                    bestScore = (value < bestScore) ? value : bestScore;
                }
            }
        }
        return bestScore;
    }
}

void findBestMove(int board[3][3], int *bestMoveX, int *bestMoveY) {
    int bestScore = -1000;
    *bestMoveX = -1;
    *bestMoveY = -1;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == EMPTY) {
                board[i][j] = PLAYER_O;
                int moveScore = minimax(board, 0, false);
                board[i][j] = EMPTY;
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
    int board[3][3] = {
        {0, 0, 0}, // Ejemplo de matriz de entrada del código de visión artificial
        {0, 0, 0}, // Aquí deberías ingresar la matriz real proveniente del código de visión artificial
        {0, 0, 0}
    };

    int player = PLAYER_X;
    int bestMoveX, bestMoveY;
    int winner;

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

    if (winner == PLAYER_X) {
        printf("¡Has ganado!\n");
    } else if (winner == PLAYER_O) {
        printf("¡La IA ha ganado!\n");
    } else {
        printf("¡Empate!\n");
    }

    return 0;
}
