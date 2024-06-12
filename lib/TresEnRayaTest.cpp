#include "TresEnRayaTest.h"



// Función para verificar si alguien ha ganado
int checkWin(int board[3][3]) {
    // Verificar filas, columnas y diagonales
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2]) {
            if (board[i][0] == PLAYER_X) return 11;
            if (board[i][0] == PLAYER_O) return 12;
        }
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i]) {
            if (board[0][i] == PLAYER_X) return 11;
            if (board[0][i] == PLAYER_O) return 12;
        }
    }
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
        if (board[0][0] == PLAYER_X) return 11;
        if (board[0][0] == PLAYER_O) return 12;
    }
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
        if (board[0][2] == PLAYER_X) return 11;
        if (board[0][2] == PLAYER_O) return 12;
    }

    // Verificar empate
    int empty = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == EMPTY) empty++;
        }
    }
    if (empty == 0) return 10;

    return 0;  // Juego en curso
}

// Función Minimax
int minimax(int board[3][3], int depth, int isMaximizing) {
    int score = checkWin(board);
    if (score != 0) return score;

    if (isMaximizing) {
        int bestScore = -1000;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == EMPTY) {
                    board[i][j] = PLAYER_X;
                    int currentScore = minimax(board, depth + 1, 0);
                    board[i][j] = EMPTY;
                    bestScore = (currentScore > bestScore) ? currentScore : bestScore;
                }
            }
        }
        return bestScore;
    } else {
        int bestScore = 1000;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == EMPTY) {
                    board[i][j] = PLAYER_O;
                    int currentScore = minimax(board, depth + 1, 1);
                    board[i][j] = EMPTY;
                    bestScore = (currentScore < bestScore) ? currentScore : bestScore;
                }
            }
        }
        return bestScore;
    }
}

// Función para encontrar la mejor jugada
int findBestMove(int board[3][3]) {
    int bestMove = -1;
    int bestScore = -1000;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == EMPTY) {
                board[i][j] = PLAYER_X;
                int moveScore = minimax(board, 0, 0);
                board[i][j] = EMPTY;
                if (moveScore > bestScore) {
                    bestScore = moveScore;
                    bestMove = i * 3 + j + 1;
                }
            }
        }
    }
    return bestMove;
}

// Función principal para determinar la jugada de la computadora
int ComputadoraMovimiento() {
    Fn_MatrizState();
    int result = checkWin(MatrizEstado);
    if (result != 0) return result;
    
    int bestMove = findBestMove(MatrizEstado);
    Serial.println(bestMove);
    
    return bestMove;
}

