#ifndef TIC_TAC_TOE_H
#define TIC_TAC_TOE_H

#include <Arduino.h>

#define N 3


extern int MatrizEstado[N][N]; //MatrizEstado se declara extern para estar disponible en todo el programa
void Fn_MatrizState();// Función para actualizar el estado de la matriz
void Fn_printMatriz(int MatrizEstadoNuevo[N][N]);

void compruebaGanador(int MatrizEstadoNuevo[N][N], int *ganador, bool *empate);
int minimax(int MatrizEstadoNuevo[N][N], int prof, bool isMax);
void movOptimo(int MatrizEstadoNuevo[N][N], int *pos_optima, int *resultado);
void convertir_tablero(int MatrizEstado[N][N], int MatrizEstadoNuevo[N][N]);
int PosicionOptima();

#endif
