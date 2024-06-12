#include "tic_tac_toe.h"


void compruebaGanador(int MatrizEstadoNuevo[N][N], int *ganador, bool *empate)
{
    int combinaciones[8][3] = {
        {0, 1, 2}, {3, 4, 5}, {6, 7, 8}, // filas
        {0, 3, 6},
        {1, 4, 7},
        {2, 5, 8}, // columnas
        {0, 4, 8},
        {2, 4, 6} // diagonales
    };

    *ganador = 0;
    *empate = true;

    for (int i = 0; i < 8; ++i)
    {
        int a = combinaciones[i][0];
        int b = combinaciones[i][1];
        int c = combinaciones[i][2];

        if (MatrizEstadoNuevo[a / N][a % N] == MatrizEstadoNuevo[b / N][b % N] &&
            MatrizEstadoNuevo[b / N][b % N] == MatrizEstadoNuevo[c / N][c % N] &&
            MatrizEstadoNuevo[a / N][a % N] != 0)
        {

            *ganador = MatrizEstadoNuevo[a / N][a % N];
            *empate = false;
            return;
        }

        if (MatrizEstadoNuevo[a / N][a % N] == 0 || MatrizEstadoNuevo[b / N][b % N] == 0 ||
            MatrizEstadoNuevo[c / N][c % N] == 0)
        {
            *empate = false;
        }
    }
}

int minimax(int MatrizEstadoNuevo[N][N], int prof, bool isMax)
{
    int ganador;
    bool empate;
    compruebaGanador(MatrizEstadoNuevo, &ganador, &empate);

    if (ganador == 1)
    {
        return 100 - prof; // Robot gana
    }
    else if (ganador == -1)
    {
        return -100 + prof; // Jugador humano gana
    }
    else if (empate)
    {
        return 0; // Empate
    }

    int mejorPuntuacion = isMax ? -1000 : 1000;

    for (int i = 0; i < N * N; ++i)
    {
        int row = i / N;
        int col = i % N;

        if (MatrizEstadoNuevo[row][col] == 0)
        {
            MatrizEstadoNuevo[row][col] = isMax ? 1 : -1;
            int puntuacion = minimax(MatrizEstadoNuevo, prof + 1, !isMax);
            MatrizEstadoNuevo[row][col] = 0;

            if (isMax && puntuacion > mejorPuntuacion)
            {
                mejorPuntuacion = puntuacion;
            }
            else if (!isMax && puntuacion < mejorPuntuacion)
            {
                mejorPuntuacion = puntuacion;
            }
        }
    }

    return mejorPuntuacion;
}

void movOptimo(int MatrizEstadoNuevo[N][N], int *pos_optima, int *resultado)
{
    int casillas_disponibles[N * N];
    int num_disponibles = 0;

    for (int i = 0; i < N * N; ++i)
    {
        int row = i / N;
        int col = i % N;

        if (MatrizEstadoNuevo[row][col] == 0)
        {
            casillas_disponibles[num_disponibles++] = i;
        }
    }

    int mejor_puntuacion = -1000;
    *pos_optima = -1;

    for (int i = 0; i < num_disponibles; ++i)
    {
        int row = casillas_disponibles[i] / N;
        int col = casillas_disponibles[i] % N;

        MatrizEstadoNuevo[row][col] = 1;
        int puntuacion = minimax(MatrizEstadoNuevo, 0, false);
        MatrizEstadoNuevo[row][col] = 0;

        if (puntuacion > mejor_puntuacion)
        {
            mejor_puntuacion = puntuacion;
            *pos_optima = casillas_disponibles[i];
        }
    }

    MatrizEstadoNuevo[*pos_optima / N][*pos_optima % N] = 1;
    int ganador;
    bool empate;
    compruebaGanador(MatrizEstadoNuevo, &ganador, &empate);

    if (ganador == 1)
    {
        *resultado = 11; // Robot ha ganado
    }
    else if (ganador == -1)
    {
        *resultado = 12; // Humano ha ganado
    }
    else if (empate)
    {
        *resultado = 13; // Empate
    }
    else
    {
        *resultado = 0; // No hay resultado definitivo
    }
}

void convertir_tablero(int MatrizEstado[N][N], int MatrizEstadoNuevo[N][N])
{
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            if (MatrizEstado[i][j] == 2)
            {
                MatrizEstadoNuevo[i][j] = 0; // Casilla vacía
            }
            else if (MatrizEstado[i][j] == 0)
            {
                MatrizEstadoNuevo[i][j] = -1; // Casilla con O (jugador humano)
            }
            else if (MatrizEstado[i][j] == 1)
            {
                MatrizEstadoNuevo[i][j] = 1; // Casilla con X (robot)
            }
        }
    }
}

int PosicionOptima()
{
    Fn_MatrizState(); // Si es necesario, llama a esta función para actualizar MatrizEstado
    // Serial.println("Matriz en posicion óptima recibida:");
    // Fn_printMatriz(int MatrizEstadoNuevo[N][N]); // Si es necesario, imprime la matriz recibida

    int MatrizEstadoNuevo[N][N];
    convertir_tablero(MatrizEstado, MatrizEstadoNuevo); // Convertir la matriz original a la representación adecuada

    int pos_optima, resultado;
    movOptimo(MatrizEstadoNuevo, &pos_optima, &resultado); // Encontrar la posición óptima

    // Si la posición óptima está entre 0 y 8, retorna esa posición
    if (pos_optima >= 0 && pos_optima <= 8)
    {
        pos_optima = pos_optima + 1; // Sumar 1 para que la posición sea del 1 al 9
    }

    // Si no hay movimientos posibles, verificar el resultado del juego
    else if (pos_optima == -1)
    {
        resultado = compruebaGanador(MatrizEstadoNuevo); // Verificar el resultado del juego
        if (resultado == 2)
        {
            pos_optima = 10; // Empate
        }
        else if (resultado == 1)
        {
            pos_optima = 11; // El robot ha ganado
        }
        else if (resultado == -1)
        {
            pos_optima = 12; // El humano ha ganado
        }
    }

    return pos_optima; // Retornar la posición óptima
}
