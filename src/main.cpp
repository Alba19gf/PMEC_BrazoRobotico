//* ALGORITMO DE JUEGO AUTOMÁTICO
// Este código implementa un juego de tres en raya (tic-tac-toe) en el que el robot calcula su mejor movimiento usando el algoritmo Minimax y realiza su jugada.
// La función Fn_MatrizState actualiza el estado del tablero desde una fuente externa, y el programa convierte ese estado a una representación interna para procesar el juego.
#include <Arduino.h>
#include <stdio.h>
#include <limits.h> // Para INT_MIN y INT_MAX

const int SIZE = 3;
int MatrizEstado[SIZE][SIZE];

void setMatrizEstadoPrueba()
{
  // Define una matriz de estado de prueba
  int matrizPrueba[SIZE][SIZE] = {{2, 1, 0}, {0, 1, 0}, {2, 2, 1}};
  // Copia la matriz de prueba a la MatrizEstado
  for (int i = 0; i < SIZE; i++)
  {
    for (int j = 0; j < SIZE; j++)
    {
      MatrizEstado[i][j] = matrizPrueba[i][j];
    }
  }
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

  return 2; // Si no hay ganador y no hay espacios vacíos, es empate
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

void setup()
{
  // Inicializar comunicación serial
  Serial.begin(115200);

  setMatrizEstadoPrueba();
  int MatrizEstadoNuevo[SIZE][SIZE];
  convertir_matrizGlobal(MatrizEstadoNuevo);
  int resultado = comprueba_ganador(MatrizEstadoNuevo);
  Serial.print("El resultado del juego es: ");
  if (resultado == 1)
  {
    Serial.println("El robot ha ganado.");
  }
  else if (resultado == -1)
  {
    Serial.println("El humano ha ganado.");
  }
  else if (resultado == 2)
  {
    Serial.println("Es un empate.");
  }
  else
  {
    Serial.println("El juego sigue en curso.");
  }
}
}
