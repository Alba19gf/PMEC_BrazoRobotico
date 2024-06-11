#ifndef TresEnRaya_h
#define TresEnRaya_h

#include <Arduino.h>
#include <stdio.h>
#include <limits.h>

#define SIZE 3

void Fn_MatrizState(int MatrizEstado[SIZE][SIZE]);
void convertir_matriz(int MatrizEstado[SIZE][SIZE], int MatrizEstadoNuevo[SIZE][SIZE]);
void comprueba_ganador(int MatrizEstadoNuevo[SIZE][SIZE], int *empate);
void minimax(int MatrizEstadoNuevo[SIZE][SIZE], int prof, int isMax, int* resultado);
void mov_optimo(int MatrizEstado[SIZE][SIZE], int* pos_optima, int* resultado);

void ejecutarJuego();

#endif
