// TresEnRaya.cpp
#include "TresEnRaya.h"

void Fn_MatrizState()
{
    // Aquí se debe actualizar la matrizEstado desde la fuente externa
}

void convertir_matriz(int MatrizEstado[SIZE][SIZE], int MatrizEstadoNuevo[SIZE][SIZE])
{
    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if (MatrizEstado[i][j] == 2)
                MatrizEstadoNuevo[i][j] = 0; // CASILLAS VACÍAS se convierten a 0
            else if (MatrizEstado[i][j] == 0)
                MatrizEstadoNuevo[i][j] = -1; // Cambia de representación 0 a -1  JUGADOR HUMANO
            else
                MatrizEstadoNuevo[i][j] = 1; // Los demás valores se mantienen como 1  JUGADOR ROBOT
        }
    }
}

int comprueba_ganador(int MatrizEstadoNuevo[SIZE][SIZE])
{
    int combinaciones[8][3][2] = {
        {{0, 0}, {0, 1}, {0, 2}}, {{1, 0}, {1, 1}, {1, 2}}, {{2, 0}, {2, 1}, {2, 2}}, // Combinaciones para filas
        {{0, 0}, {1, 0}, {2, 0}},
        {{0, 1}, {1, 1}, {2, 1}},
        {{0, 2}, {1, 2}, {2, 2}}, // Combinaciones para columnas
        {{0, 0}, {1, 1}, {2, 2}},
        {{0, 2}, {1, 1}, {2, 0}} // Combinaciones para diagonales
    };

    for (int i = 0; i < 8; i++)
    {
        int a = combinaciones[i][0][0], b = combinaciones[i][0][1];
        int c = combinaciones[i][1][0], d = combinaciones[i][1][1];
        int e = combinaciones[i][2][0], f = combinaciones[i][2][1];

        if (MatrizEstadoNuevo[a][b] != 0 && MatrizEstadoNuevo[a][b] == MatrizEstadoNuevo[c][d] && MatrizEstadoNuevo[a][b] == MatrizEstadoNuevo[e][f])
        {
            return MatrizEstadoNuevo[a][b]; // Devuelve 1 si gana el robot, -1 si gana el humano
        }
    }

    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if (MatrizEstadoNuevo[i][j] == 0)
            {
                return 0; // No es empate si hay un espacio vacío
            }
        }
    }

    return 1; // Si no hay ganador y no hay espacios vacíos, es empate
}

int minimax(int MatrizEstadoNuevo[SIZE][SIZE], int prof, int isMax)
{
    int empate = comprueba_ganador(MatrizEstadoNuevo);

    if (empate)
    {
        return 0;
    }

    if (isMax)
    {
        int mejorPuntuacion = INT_MIN;
        for (int i = 0; i < SIZE; i++)
        {
            for (int j = 0; j < SIZE; j++)
            {
                if (MatrizEstadoNuevo[i][j] == 0)
                {
                    MatrizEstadoNuevo[i][j] = 1;
                    int puntuacion = minimax(MatrizEstadoNuevo, prof + 1, 0);
                    MatrizEstadoNuevo[i][j] = 0;
                    mejorPuntuacion = max(mejorPuntuacion, puntuacion);
                }
            }
        }
        return mejorPuntuacion;
    }
    else
    {
        int mejorPuntuacion = INT_MAX;
        for (int i = 0; i < SIZE; i++)
        {
            for (int j = 0; j < SIZE; j++)
            {
                if (MatrizEstadoNuevo[i][j] == 0)
                {
                    MatrizEstadoNuevo[i][j] = -1;
                    int puntuacion = minimax(MatrizEstadoNuevo, prof + 1, 1);
                    MatrizEstadoNuevo[i][j] = 0;
                    mejorPuntuacion = min(mejorPuntuacion, puntuacion);
                }
            }
        }
        return mejorPuntuacion;
    }
}

int mov_optimo(int MatrizEstado[SIZE][SIZE])
{
    int MatrizEstadoNuevo[SIZE][SIZE];
    convertir_matriz(MatrizEstado, MatrizEstadoNuevo);

    int mejor_puntuacion = INT_MIN;
    int mejor_casilla = -1;

    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if (MatrizEstadoNuevo[i][j] == 0)
            {
                MatrizEstadoNuevo[i][j] = 1;
                int puntuacion = minimax(MatrizEstadoNuevo, 0, 0);
                MatrizEstadoNuevo[i][j] = 0;

                if (puntuacion > mejor_puntuacion)
                {
                    mejor_puntuacion = puntuacion;
                    mejor_casilla = i * SIZE + j; // Calcular la posición óptima
                }
            }
        }
    }

    return mejor_casilla;
}

int PosicionOptima()
{
    // Llamar a la función externa para actualizar MatrizEstado
    int MatrizEstado[SIZE][SIZE];
    Fn_MatrizState();

    // Convertir MatrizEstado a MatrizEstadoNuevo
    int MatrizEstadoNuevo[SIZE][SIZE];
    convertir_matriz(MatrizEstado, MatrizEstadoNuevo);

    // Obtener la posición óptima
    return mov_optimo(MatrizEstadoNuevo); //En caso de haber un ganador o empate, mov=optimo=-1
}

int ResultadoJuego()
{
    // Llamar a la función externa para actualizar MatrizEstado
    int MatrizEstado[SIZE][SIZE];
    Fn_MatrizState();

    // Convertir MatrizEstado a MatrizEstadoNuevo
    int MatrizEstadoNuevo[SIZE][SIZE];
    convertir_matriz(MatrizEstado, MatrizEstadoNuevo);

    // Comprobar el resultado y asignar el valor adecuado
    int resultadoJuego = comprueba_ganador(MatrizEstadoNuevo);
    if (resultadoJuego == 2)
    {
        resultadoJuego = 13; // Empate
    }
    else if (resultadoJuego == 1)
    {
        resultadoJuego = 12; // El robot ha ganado
    }
    else if (resultadoJuego == -1)
    {
        resultadoJuego = 13; // El humano ha ganado
    }
    else if (resultadoJuego == 0)
    {
        resultadoJuego = 10; // El juego no ha terminado
    }

    return resultadoJuego;
}
