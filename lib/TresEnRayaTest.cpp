#include "TresEnRayaTest.h"

int Movefrom();

//============================ MODO DIFICIL============================================================================================    
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

// Función Minimax con poda alfa-beta
int minimax(int board[3][3], int depth, int alpha, int beta, int isMaximizing) {
    int score = checkWin(board);
    if (score != 0) {
        if (score == 11) return 10 - depth;  // Gana la computadora
        if (score == 12) return depth - 10;  // Gana el humano
        if (score == 10) return 0;           // Empate
    }

    if (isMaximizing) {
        int bestScore = INT_MIN;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == EMPTY) {
                    board[i][j] = PLAYER_X;
                    int currentScore = minimax(board, depth + 1, alpha, beta, 0);
                    board[i][j] = EMPTY;
                    bestScore = (currentScore > bestScore) ? currentScore : bestScore;
                    alpha = (alpha > bestScore) ? alpha : bestScore;
                    if (beta <= alpha) break;
                }
            }
        }
        return bestScore;
    } else {
        int bestScore = INT_MAX;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == EMPTY) {
                    board[i][j] = PLAYER_O;
                    int currentScore = minimax(board, depth + 1, alpha, beta, 1);
                    board[i][j] = EMPTY;
                    bestScore = (currentScore < bestScore) ? currentScore : bestScore;
                    beta = (beta < bestScore) ? beta : bestScore;
                    if (beta <= alpha) break;
                }
            }
        }
        return bestScore;
    }
}

// Función para encontrar la mejor jugada
int findBestMove(int board[3][3]) {
    int bestMove = -1;
    int bestScore = INT_MIN;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == EMPTY) {
                board[i][j] = PLAYER_X;
                int moveScore = minimax(board, 0, INT_MIN, INT_MAX, 0);
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
    
    Serial.println("Modo dificil");
    Fn_MatrizState();
    int from;                                    //variable para enviar a la funcion de cinematica
    from=Movefrom(0);
    int result = checkWin(MatrizEstado);
    if (result != 0) return result;
    int bestMove = findBestMove(MatrizEstado);      //variable para enviar a la funcion de cinematica
    Moving=0;
    Mover_pieza(from,bestMove);

    if (Moving==1){
            return bestMove;
    }

}
//  ======================================== Modo robot facil==========================================================================

// Función para verificar si alguien ha ganado
int checkWinsf(int board[3][3]) {
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
int minimaxsf(int board[3][3], int depth, int isMaximizing) {
    int score = checkWinsf(board);
    if (score != 0) return score;

    if (isMaximizing) {
        int bestScore = -1000;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == EMPTY) {
                    board[i][j] = PLAYER_X;
                    int currentScore = minimaxsf(board, depth + 1, 0);
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
                    int currentScore = minimaxsf(board, depth + 1, 1);
                    board[i][j] = EMPTY;
                    bestScore = (currentScore < bestScore) ? currentScore : bestScore;
                }
            }
        }
        return bestScore;
    }
}

// Función para encontrar la mejor jugada
int findBestMovesf(int board[3][3]) {
    int bestMove = -1;
    int bestScore = -1000;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == EMPTY) {
                board[i][j] = PLAYER_X;
                int moveScore = minimaxsf(board, 0, 0);
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
int ComputadoraMovimientoEasy() {
    Serial.println("Modo facil");
    Fn_MatrizState();
    int from;
    from=Movefrom(0);
    int result = checkWinsf(MatrizEstado);
    if (result != 0) return result;
    int bestMove = findBestMovesf(MatrizEstado);

    Moving=0;
    Mover_pieza(from,bestMove);

    if (Moving==1){
        return bestMove;
    }

    // if (bestMove<=9){
    //    // Serial.print("Colocar la pieza en la casilla: ");
    //     //Serial.println(bestMove);
    // }

    // if (Moving==1){
    //     Moving=Fn_readSerial();
    //     if (Moving==0){
    //          return bestMove;
    //     }
    // }  
}



int Movefrom(int ficha)                 //ficha 0 para azul O         1 para rosa X
{
    int blue=1, pink=0;
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        if(MatrizEstado[i][j]==0) blue++;
        else if (MatrizEstado[i][j]==1) pink++;
      }
    }
    //Serial.print("Tomar pieza de: ");
    

    if (ficha==0){
        return blue;
        Serial.println(blue);
    }

    else if (ficha==1){
        return (pink+6);
        Serial.println(pink+6);
    }
    
}


int Mover_pieza(int ini, int final){
    digitalWrite(led_red, HIGH);
    digitalWrite(led_green, LOW);
    Serial.print("Mover desde: ");
    Serial.print(ini);
    Serial.print("Mover hasta: ");
    Serial.print(final);
    if (Moving==0){
        Moving=Fn_readSerial();
    if (Moving==1){
        return 1;
    }
    }  
    

}

