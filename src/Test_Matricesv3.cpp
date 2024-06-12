#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <vector>
#include <iostream>
#include "../lib/FnKinematics/FnKinematics.cpp"
#include "../lib/FnKinematics/FnKinematicsv2.cpp"

struct nums pos1, pos2, pos3, pos4, pos5, pos6, pos7, pos8, pos9, rob1, rob2, rob3, rob4, rob5, hum1, hum2, hum3, hum4, hum5;

int main() {
    pos1.uno = -39.25;
    pos1.dos = 290.75;
    pos1.tres = -145;

    pos2.uno = 0;
    pos2.dos = 290.75;
    pos2.tres = -145;

    pos3.uno = 39.25;
    pos3.dos = 290.75;
    pos3.tres = -145;

    pos4.uno = -39.25;
    pos4.dos = 330;
    pos4.tres = -145;

    pos5.uno = 0;
    pos5.dos = 330;
    pos5.tres = -145;

    pos6.uno = 39.25;
    pos6.dos = 330;
    pos6.tres = -145;

    pos7.uno = -39.25;
    pos7.dos = 369.25;
    pos7.tres = -145;

    pos8.uno = 0;
    pos8.dos = 369.25;
    pos8.tres = -145;

    pos9.uno = 39.25;
    pos9.dos = 369.25;
    pos9.tres = -145;

    rob1.uno = -70;
    rob1.dos = 260;
    rob1.tres = -145;

    rob2.uno = -35;
    rob2.dos = 260;
    rob2.tres = -145;

    rob3.uno = 0;
    rob3.dos = 260;
    rob3.tres = -145;

    rob4.uno = 35;
    rob4.dos = 260;
    rob4.tres = -145;

    rob5.uno = 70;
    rob5.dos = 260;
    rob5.tres = -145;

    hum1.uno = -70;
    hum1.dos = 400;
    hum1.tres = -145;

    hum2.uno = -35;
    hum2.dos = 400;
    hum2.tres = -145;

    hum3.uno = 0;
    hum3.dos = 400;
    hum3.tres = -145;

    hum4.uno = 35;
    hum4.dos = 400;
    hum4.tres = -145;

    hum5.uno = 70;
    hum5.dos = 400;
    hum5.tres = -145;

    int mov_optimo; // Esta será la variable que nos devuelva el algoritmo de juego
    struct nums pos_obj;

    printf("Introduzca la posicion objetivo del tablero (1-9): ");
    scanf("%d", &mov_optimo);

    if (mov_optimo == 1) {
        pos_obj = pos1;
        printf("La posicion objetivo es la 1\n");
    }
    else if (mov_optimo == 2) {
        pos_obj = pos2;
        printf("La posicion objetivo es la 2\n");
    }
    else if (mov_optimo == 3) {
        pos_obj = pos3;
        printf("La posicion objetivo es la 3\n");
    }
    else if (mov_optimo == 4) {
        pos_obj = pos4;
        printf("La posicion objetivo es la 4\n");
    }
    else if (mov_optimo == 5) {
        pos_obj = pos5;
        printf("La posicion objetivo es la 5\n");
    }
    else if (mov_optimo == 6) {
        pos_obj = pos6;
        printf("La posicion objetivo es la 6\n");
    }
    else if (mov_optimo == 7) {
        pos_obj = pos7;
        printf("La posicion objetivo es la 7\n");
    }
    else if (mov_optimo == 8) {
        pos_obj = pos8;
        printf("La posicion objetivo es la 8\n");
    }
    else if (mov_optimo == 9) {
        pos_obj = pos9;
        printf("La posicion objetivo es la 9\n");
    }
    // Se va a necesitar una variable en el programa principal que recorra el vector de posiciones
    // iniciales de las piezas (no sería rob1, sino rob en función de esta variable)

    printf("\n");

    float** angulos1;
    float** angulos2;
    float** angulos3;

    // Generación del camino desde la posición de reposo hasta la aproximación de la posición de la pieza
    // Espera
    // LLamada a la función que cierra la pinza
    angulos1 = arrayTLinealv2(rob1.uno, rob1.dos, rob1.tres, rob1.uno, rob1.dos, rob1.tres+40);
    angulos2 = arrayTLinealv2(rob1.uno, rob1.dos, rob1.tres+40, pos_obj.uno, pos_obj.dos, pos_obj.tres+40);
    angulos3 = arrayTLinealv2(pos_obj.uno, pos_obj.dos, pos_obj.tres+40, pos_obj.uno, pos_obj.dos, pos_obj.tres);
    // Espera
    // LLamada a la función que abre la pinza
    // Vuelta a la posición de reposo
}