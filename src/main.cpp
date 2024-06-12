//* ALGORITMO DE JUEGO AUTOMÁTICO
// Este código implementa un juego de tres en raya (tic-tac-toe) en el que el robot calcula su mejor movimiento usando el algoritmo Minimax y realiza su jugada.
// La función Fn_MatrizState actualiza el estado del tablero desde una fuente externa, y el programa convierte ese estado a una representación interna para procesar el juego.

#include <Arduino.h>
#include <stdio.h>
#include <limits.h> // Para INT_MIN y INT_MAX

#define SIZE 3 // Definir tablero de 3x3

// Declarar la matriz global ENTRADA DE VISION
int MatrizEstado[SIZE][SIZE];

// Declarar la función externa para recibir la matriz de visión
extern void Fn_MatrizState();

void convertir_matrizGlobal(int MatrizEstadoNuevo[SIZE][SIZE])
{
  Serial.println("Convirtiendo matriz de estado Global");

  for (int i = 0; i < SIZE; i++)
  {
    for (int j = 0; j < SIZE; j++)
    {
      if (MatrizEstado[i][j] == 2)
        MatrizEstadoNuevo[i][j] = 0; // CASILLAS VACÍAS se convierten a 0
      else if (MatrizEstado[i][j] == 0)
        MatrizEstadoNuevo[i][j] = -1; // Cambia de representación 0 a -1 JUGADOR HUMANO
      else
        MatrizEstadoNuevo[i][j] = 1; // Los demás valores se mantienen como 1 JUGADOR ROBOT
    }
  }
  Fn_printMatriz(MatrizEstadoNuevo);
}

void convertir_matrizGlobal(int MatrizEstadoNuevo[SIZE][SIZE])
{
  Serial.println("Convirtiendo matriz de estado Global");

  for (int i = 0; i < SIZE; i++)
  {
    for (int j = 0; j < SIZE; j++)
    {
      if (MatrizEstado[i][j] == 2)
        MatrizEstadoNuevo[i][j] = 0; // CASILLAS VACÍAS se convierten a 0
      else if (MatrizEstado[i][j] == 0)
        MatrizEstadoNuevo[i][j] = -1; // Cambia de representación 0 a -1 JUGADOR HUMANO
      else
        MatrizEstadoNuevo[i][j] = 1; // Los demás valores se mantienen como 1 JUGADOR ROBOT
    }
  }
  Fn_printMatriz(MatrizEstadoNuevo);
}

int minimax(int MatrizEstadoNuevo[SIZE][SIZE], int prof, bool isMax)
{
  int ganador = comprueba_ganador(MatrizEstadoNuevo);

  if (ganador == 1)
    return 100 - prof; // Robot gana
  if (ganador == -1)
    return -100 + prof; // Jugador humano gana
  if (ganador == 2)     // Empate
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
  convertir_matrizGlobal(MatrizEstadoNuevo);
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

// int PosicionOptima()
// {
//   Fn_MatrizState();
//   // Llamar a la función externa para actualizar MatrizEstado
//   Serial.println("Matriz en posicion óptima recibida:");
//   Fn_printMatriz(MatrizEstado);

//   int MatrizMinimax[SIZE][SIZE];
//   convertir_matrizGlobal(MatrizMinimax); // Llamada para convertir la matriz global

//   // Obtener la posición óptima
//   int pos_optima = mov_optimo();

//   // Si la posición óptima está entre 0 y 8, retornar esa posición
//   if (pos_optima >= 0 && pos_optima <= 8)
//   {
//     pos_optima = pos_optima + 1;
//   }

//   // Si no hay movimientos posibles, verificar el resultado del juego
//   else if (pos_optima == -1)
//   {
//     int resultado = comprueba_ganador(MatrizMinimax);
//     if (resultado == 2)
//     {
//       pos_optima = 10; // Empate
//     }
//     else if (resultado == 1)
//     {
//       pos_optima = 11; // El robot ha ganado
//     }
//     else if (resultado == -1)
//     {
//       pos_optima = 12; // El humano ha ganado
//     }
//   }

//   return pos_optima;
//}

void setup()
{
  // Inicializar comunicación serial
  Serial.begin(115200);
  Fn_MatrizState();
  // Llamar a la función externa para actualizar MatrizEstado
  Serial.println("Matriz en posicion óptima recibida:");
  Fn_printMatriz(MatrizEstado);

  int MatrizMinimax[SIZE][SIZE];
  convertir_matrizGlobal(MatrizMinimax); // Llamada para convertir la matriz global

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

  return pos_optima;
}
