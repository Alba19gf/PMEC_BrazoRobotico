#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <vector>
#include <iostream>
#include "../lib/FnKinematics/FnKinematics.cpp"

int main() { 

    float** angulost;
    angulost = arrayTLineal(-80, 250, -140, 80, 250, -140);
    
    for(int i = 0; i < num_pasos; i++) {
        printf("TLINEAL %i: %f\t %f\t %f\n", i, angulost[i][0], angulost[i][1], angulost[i][2]);
    }
}
