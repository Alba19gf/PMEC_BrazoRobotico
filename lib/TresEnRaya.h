#ifndef TresEnRaya_h
#define TresEnRaya_h

#include <Arduino.h>

class TresEnRaya {
public:
    TresEnRaya();
    void inicializar();
    int obtenerPosicionOptima();
    int obtenerResultado();

private:
    void convertirMatriz(int MatrizEstado[SIZE][SIZE], int MatrizEstadoNuevo[SIZE][SIZE]);
    int compruebaGanador(int MatrizEstadoNuevo[SIZE][SIZE], int *empate);
    int minimax(int MatrizEstadoNuevo[SIZE][SIZE], int prof, int isMax);
    int movOptimo(int MatrizEstadoNuevo[SIZE][SIZE]);

    int SIZE;
    int **MatrizEstado;
    int resultado;
};

#endif
