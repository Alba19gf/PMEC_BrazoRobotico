// TresEnRaya.cpp
#include "TresEnRaya.h"

int convertir_matrizGlobal()
{
    int MatrizEstadoNuevo[SIZE][SIZE];
    Serial.println("Convirtiendo matriz de estado Global");

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
    Fn_printMatriz(MatrizEstadoNuevo);
    return MatrizEstadoNuevo[SIZE][SIZE];
}

int comprueba_ganador(int MatrizEstadoNuevo[SIZE][SIZE])
{
    Serial.println("Comprobando ganador");
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
            Serial.println("GANO");
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

    return 2; // Si no hay ganador y no hay espacios vacíos, es empate
    Serial.println("EMPATE");
}

int minimax(int MatrizEstadoNuevo[SIZE][SIZE], int prof, bool isMax)
{
    int ganador = comprueba_ganador(MatrizEstadoNuevo);

    if (ganador == 1)
        return 100 - prof; // Robot gana
    if (ganador == -1)
        return -100 + prof; // Jugador humano gana
    if (ganador == 2)       // Empate
        return 0;

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
                    int puntuacion = minimax(MatrizEstadoNuevo, prof + 1, false);
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
                    int puntuacion = minimax(MatrizEstadoNuevo, prof + 1, true);
                    MatrizEstadoNuevo[i][j] = 0;
                    mejorPuntuacion = min(mejorPuntuacion, puntuacion);
                }
            }
        }
        return mejorPuntuacion;
    }
}

int mov_optimo()
{
    int MatrizEstadoNuevo[SIZE][SIZE];
    MatrizEstadoNuevo[SIZE][SIZE]=convertir_matrizGlobal();
    Serial.println("Matriz convertida a Minimax, en mov optimo");
    Fn_printMatriz(MatrizEstadoNuevo);

    int mejor_puntuacion = INT_MIN;
    int mejor_casilla = -1;

    for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            if (MatrizEstadoNuevo[i][j] == 0)
            {
                MatrizEstadoNuevo[i][j] = 1;
                int puntuacion = minimax(MatrizEstadoNuevo, 0, false);
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
    Fn_MatrizState();
    // Llamar a la función externa para actualizar MatrizEstado
    Serial.println("Matriz en posicion oprtima recibida:");
    Fn_printMatriz(MatrizEstado);

    int MatrizMinimax[3][3];
    MatrizMinimax[3][3] = convertir_matrizGlobal();

    // Obtener la posición óptima
    int pos_optima = mov_optimo();

    // Si la posición óptima está entre 0 y 8, retornar esa posición
    if (pos_optima >= 0 && pos_optima <= 8)
    {
        pos_optima = pos_optima + 1;
    }

    // Si no hay movimientos posibles, verificar el resultado del juego
    else if (pos_optima == -1)
    {
        if (pos_optima == -1)
        {
            int resultado = comprueba_ganador(MatrizMinimax);
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
    }

    return pos_optima;
}
