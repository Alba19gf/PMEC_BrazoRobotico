#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <vector>
#include <iostream>
#include "../lib/FnKinematics/FnKinematics.cpp"

int main() { 

    //float** angulost_lin = arrayTLineal(-80, 250, -140, 80, 250, -140);
    
    //for(int i = 0; i < num_pasos; i++) {
    //    printf("TLINEAL %i: %f\t %f\t %f\n", i, angulost_lin[i][0], angulost_lin[i][1], angulost_lin[i][2]);
    //}

    //float** angulost_elip = arrayTelipse(-70, 260, 39.25, 369.25);
    
    //for (int i = 0; i < num_pasos; i++) {
    //    printf("TELIPSE %d:\t%2.3f\t %2.3f\t %2.3f\n", i, angulost_elip[i][0], angulost_elip[i][1], angulost_elip[i][2]);
    //}

    //struct nums angulos_calibracion = FnInvKinem(0, 330, -140);

    //printf("Angulo base: %2.3f Angulo hombro: %2.3f Angulo codo: %2.3f", angulos_calibracion.uno, angulos_calibracion.dos, angulos_calibracion.tres);

    struct nums angulos_pruebas = FnInvKinem(39.25, 369.25, -45);

    printf("Angulo base: %2.3f Angulo hombro: %2.3f Angulo codo: %2.3f", angulos_pruebas.uno, angulos_pruebas.dos, angulos_pruebas.tres);
}