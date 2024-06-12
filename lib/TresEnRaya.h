#ifndef TresEnRaya_h
#define TresEnRaya_h

#include <Arduino.h>
#include <stdio.h>
#include <limits.h>
#define SIZE 3
// const int SIZE = 3;

extern int MatrizEstado[SIZE][SIZE]; //MatrizEstado se declara extern para estar disponible en todo el programa


void Fn_MatrizState();// Función para actualizar el estado de la matriz
void Fn_printMatriz(int MatrizEstadoNuevo[SIZE][SIZE]);
void convertir_matrizGlobal(int MatrizEstadoNuevo[SIZE][SIZE]);

int comprueba_ganador(int MatrizEstadoNuevo[SIZE][SIZE]);
int minimax(int MatrizEstadoNuevo[SIZE][SIZE], int prof, bool isMax);
int mov_optimo();

int PosicionOptima();


#endif
