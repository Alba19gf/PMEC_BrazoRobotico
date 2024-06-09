#ifndef ROBOT_H
#define ROBOT_H

#include "Kinematics.h"
#include <vector>

class Robot {
public:
    void Tlineal(float x_inicio, float y_inicio, float z_inicio, float x_fin, float y_fin, float z_fin);
    void Telipse(float x_inicio, float y_inicio, float x_fin, float y_fin);

    float** arrayTlineal(float x_inicio, float y_inicio, float z_inicio, float x_fin, float y_fin, float z_fin);
    float** arrayTelipse(float x_inicio, float y_inicio, float x_fin, float y_fin);
};

#endif // ROBOT_H