#ifndef TresEnRaya_h
#define TresEnRaya_h

#include <Arduino.h>
#include <stdio.h>
#include <limits.h>
extern int MatrizEstado[3][3]; //MatrizEstado se declara extern para estar disponible en todo el programa
#define SIZE 3

void Fn_printMatriz(int MatrizEstadoNuevo[SIZE][SIZE]);
int convertir_matrizGlobal(int MatrizEstado[SIZE][SIZE], int MatrizEstadoNuevo[SIZE][SIZE]);

int comprueba_ganador(int MatrizEstadoNuevo[SIZE][SIZE], int *empate);
int minimax(int MatrizEstadoNuevo[SIZE][SIZE], int prof, int isMax, int* resultado);
int mov_optimo();

int PosicionOptima();


#endif
