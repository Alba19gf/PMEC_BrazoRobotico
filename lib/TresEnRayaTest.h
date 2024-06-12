#ifndef TresEnRayaTest_h
#define TresEnRayaTest_h

#include "LcdMenu.h"

#include <Arduino.h>
#include <stdio.h>
#include <limits.h>
extern int MatrizEstado[3][3];
#define SIZE 3

#define PLAYER_O 0  // Humano
#define PLAYER_X 1  // Computadora
#define EMPTY 2     // Casilla vacía

int ComputadoraMovimiento();

#endif
