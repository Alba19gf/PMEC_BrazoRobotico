#ifndef TresEnRaya_h
#define TresEnRaya_h

#include <Arduino.h>
#include <stdio.h>
#include <limits.h>

class TresEnRaya {
  private:
    enum { SIZE = 3 };
    int MatrizEstado[SIZE][SIZE];
    void convertir_matriz(int MatrizEstado[SIZE][SIZE], int MatrizEstadoNuevo[SIZE][SIZE]);

  public:
    TresEnRaya();
    void Fn_MatrizState(); // Modificada para actualizar la matrizEstado
    int mov_optimo();
    int comprueba_ganador(int MatrizEstadoNuevo[SIZE][SIZE], int *empate);
    int minimax(int MatrizEstadoNuevo[SIZE][SIZE], int prof, int isMax);
};

#endif
