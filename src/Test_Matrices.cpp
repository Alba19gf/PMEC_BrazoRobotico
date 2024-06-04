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

    // ESTO SERÍA TRATANDO LA SALIDA COMO UN VECTOR DE ÁNGULOS
    // std::vector<float> angulos1(3*num_pasos);

    // angulos1 = Telipse(rob1.uno, rob1.dos, pos3.uno, pos3.dos);

    // printf("TELIPSE_pri: %f\t %f\t %f\t %f\t %f\t %f\t %f\t %f\t %f\n", angulos1[0], angulos1[1], angulos1[2], angulos1[3], angulos1[4], angulos1[5], angulos1[6], angulos1[7], angulos1[8]);
    // printf("TELIPSE_ult: %f\t %f\t %f\t %f\t %f\t %f\t %f\t %f\t %f\n", angulos1[142], angulos1[143], angulos1[144], angulos1[145], angulos1[146], angulos1[147], angulos1[148], angulos1[149], angulos1[150]);

    // VAMOS A HACERLO AHORA TRABAJANDO CON UNA MATRIZ
    float** angulos2; // Importante, las variables tipo float** declararlas en líneas diferentes, sino da problema
    float** angulos3;

    angulos2 = arrayTLineal(rob1.uno, rob1.dos, rob1.tres, pos9.uno, pos9.dos, pos9.tres);

    //for (int i = 0; i < num_pasos; i++) {
    //    printf("TLINEAL %d: %2.3f \t %2.3f \t %2.3f \n", i, angulos2[i][0], angulos2[i][1], angulos2[i][2]);
    //}

    angulos3 = arrayTelipse(rob1.uno, rob1.dos, pos9.uno, pos9.dos);
    
     for (int i = 0; i < num_pasos; i++) {
          printf("TELIPSE %d: %2.3f \t %2.3f \t %2.3f \n", i, angulos3[i][0], angulos3[i][1], angulos3[i][2]);
    }
}