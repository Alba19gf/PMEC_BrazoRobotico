#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <vector>
#include <iostream>
#include "../lib/FnKinematics/FnKinematics.cpp"

int main() { 
    std::vector<float> angulos1(3*num_pasos), angulos2(3*num_pasos);

    angulos1 = Tlineal(0, 230, -140, 50, 300, 20);
    angulos2 = Telipse(-80, 250, 50, 300);

    printf("TLINEAL: %f\t %f\t %f\t %f\t %f\t %f\t %f\t %f\t %f\n", angulos1[0], angulos1[1], angulos1[2], angulos1[3], angulos1[4], angulos1[5], angulos1[6], angulos1[7], angulos1[8]);
    printf("TELIPSE: %f\t %f\t %f\t %f\t %f\t %f\t %f\t %f\t %f\n", angulos2[0], angulos2[1], angulos2[2], angulos2[3], angulos2[4], angulos2[5], angulos2[6], angulos2[7], angulos2[8]);
}
