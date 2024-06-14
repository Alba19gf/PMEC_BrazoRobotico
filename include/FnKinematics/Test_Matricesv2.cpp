#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <vector>
#include <iostream>
#include "FnKinematics.cpp"
#include "FnKinematicsv2.cpp"

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

    
    // ASÍ SE COMPRUEBA QUE FUNCIONA CON PASOS PEQUEÑOS Y CONSIGUE LAS POSICIONES DESEADAS EN LOS EXTREMOS
    float** angulos1;
    float** angulos2;
    float** angulos3;

    angulos1 = arrayTLinealv2(rob1.uno, rob1.dos, rob1.tres, rob1.uno, rob1.dos, rob1.tres+40);
    angulos2 = arrayTLinealv2(rob1.uno, rob1.dos, rob1.tres+40, pos9.uno, pos9.dos, pos9.tres+40);
    //angulos2 = arrayTelipsev2(rob1.uno, rob1.dos, pos9.uno, pos9.dos);
    angulos3 = arrayTLinealv2(pos9.uno, pos9.dos, pos9.tres+40, pos9.uno, pos9.dos, pos9.tres);

    struct nums posi1 = FnInvKinem(rob1.uno, rob1.dos, rob1.tres);
    printf("\n \nANGULOS POS1: %2.3f \t %2.3f \t %2.3f \n", posi1.uno, posi1.dos, posi1.tres);
    struct nums posi2 = FnInvKinem(rob1.uno, rob1.dos, rob1.tres+40);
    printf("ANGULOS POS2: %2.3f \t %2.3f \t %2.3f \n", posi2.uno, posi2.dos, posi2.tres);
    struct nums posi3 = FnInvKinem(pos9.uno, pos9.dos, pos9.tres+40);
    printf("ANGULOS POS3: %2.3f \t %2.3f \t %2.3f \n", posi3.uno, posi3.dos, posi3.tres);
    struct nums posi4 = FnInvKinem(pos9.uno, pos9.dos, pos9.tres);
    printf("ANGULOS POS4: %2.3f \t %2.3f \t %2.3f \n", posi4.uno, posi4.dos, posi4.tres);
}