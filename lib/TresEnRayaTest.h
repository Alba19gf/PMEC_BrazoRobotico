#ifndef TresEnRayaTest_h
#define TresEnRayaTest_h

#include "LcdMenu.h"

#include <Arduino.h>
#include <stdio.h>
#include <limits.h>
extern int MatrizEstado[3][3];
#define SIZE 3

#define PLAYER_X 0  // Humano
#define PLAYER_O 1  // Computadora
#define EMPTY 2     // Casilla vacía




int ComputadoraMovimiento();
int findBestMove(int board[3][3]);
int minimax(int board[3][3], int depth, int alpha, int beta, int isMaximizing);
int checkWin(int board[3][3]);


int ComputadoraMovimientoEasy(); 
int findBestMovesf(int board[3][3]);
int minimaxsf(int board[3][3], int depth, int isMaximizing) ;
int checkWinsf(int board[3][3]);

#endif
