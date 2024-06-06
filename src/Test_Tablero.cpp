#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <vector>
#include <iostream>
#include "../lib/FnKinematics/FnKinematics.cpp"

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

    // Se almacenan las posiciones iniciales de las piezas en dos vectores
    std::vector<nums> pos_rob{rob1, rob2, rob3, rob4, rob5};
    std::vector<nums> pos_hum{hum1, hum2, hum3, hum4, hum5};

    // Se almacenan las posiciones del tablero en una matriz 3x3
    struct nums** pos_tab = new struct nums*[3];
    for (int i = 0; i < 3; ++i) {
        pos_tab[i] = new struct nums[3];
    }

    pos_tab[2][0] = pos1;
    pos_tab[2][1] = pos2;
    pos_tab[2][2] = pos3;
    pos_tab[1][0] = pos4;
    pos_tab[1][1] = pos5;
    pos_tab[1][2] = pos6;
    pos_tab[0][0] = pos7;
    pos_tab[0][1] = pos8;
    pos_tab[0][2] = pos9;

    printf("La posicion de pieza 2 del robot en Y es: %2.3f \n", pos_rob[1].dos);
    printf("La posicion de pieza 5 del humano en X es: %2.3f \n", pos_hum[4].uno);
    printf("La posicion de pieza 9 en el tablero es X: %2.3f \t Y: %2.3f \t Z: %2.3f", pos_tab[0][2].uno, pos_tab[0][2].dos, pos_tab[0][2].tres);
}