#ifndef ROBOT_H
#define ROBOT_H

#include "Kinematics.h"
#include <vector>

class Robot {
public:
    void Tlineal(float x_inicio, float y_inicio, float z_inicio, float x_fin, float y_fin, float z_fin, float L1, float L2, float L3);
    void Telipse(float x_inicio, float y_inicio, float x_fin, float y_fin, float L1, float L2, float L3);
    void Rest(float x_inicio, float y_inicio, float L1, float L2, float L3);
    void Approach(float x, float y, float L1, float L2, float L3);
    void MovRobot(float x_inicio, float y_inicio, float x_fin, float y_fin, float L1, float L2, float L3);
};

#endif // ROBOT_H
