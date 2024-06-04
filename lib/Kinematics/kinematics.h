#ifndef KINEMATICS_H
#define KINEMATICS_H

#include "Nums.h"
#include <cmath>

#define PI 3.141592

class Kinematics {
public:
    static Nums FnDirKinem(float ang_b, float ang_s, float ang_e, float L1, float L2, float L3);
    static Nums FnInvKinem(float x, float y, float z, float L1, float L2, float L3);
};

#endif // KINEMATICS_H
