#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../lib/FnKinematics/FnKinematics.h"

int main() {
    float xTCP, yTCP, zTCP, ang_b, ang_s, ang_e;

    struct nums posTCP = FnDirKinem((float) 20.0, (float) 10.0, (float) 80.0, (float) 275.0, (float) 255.0, (float) 180.0);
    struct nums GDL = FnInvKinem((float) 103.547684, (float) 284.494995, (float) 90.822144, (float) 275.0, (float) 255.0, (float) 180.0);

    xTCP = posTCP.uno;
    yTCP = posTCP.dos;
    zTCP = posTCP.tres;

    ang_b = GDL.uno;
    ang_s = GDL.dos;
    ang_e = GDL.tres;

    printf("\nLa posicion del TCP es: X=%f \t Y=%f \t Z=%f \n", xTCP, yTCP, zTCP);
    printf("Los angulos son: BASE=%f \t HOMBRO=%f \t CODO=%f \n", ang_b, ang_s, ang_e);
}