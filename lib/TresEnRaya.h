#ifndef TresEnRaya_h
#define TresEnRaya_h

#include <Arduino.h>
#include <stdio.h>
#include <limits.h>
extern int MatrizEstado[3][3];
#define SIZE 3


void convertir_matriz(int MatrizEstado[SIZE][SIZE], int MatrizEstadoNuevo[SIZE][SIZE]);
void comprueba_ganador(int MatrizEstadoNuevo[SIZE][SIZE], int *empate);
void minimax(int MatrizEstadoNuevo[SIZE][SIZE], int prof, int isMax, int* resultado);
int mov_optimo();

int PosicionOptima();


#endif
