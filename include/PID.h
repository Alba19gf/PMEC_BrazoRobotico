#ifndef PID_h
#define PID_h

#include "Arduino.h"

class PID 
{
    public:
        PID();
        PID(float Kp, float Ki, float Kd);
        PID(float Kp, float Ki, float Kd, float dt, float max, float min);
        int setKP(float Kp);
        int setKI(float Ki);
        int setKD(float Kd);
        int setdt(float dt);
        int setMax(float max);
        int setMin(float min);
        int setK(float Kp, float Ki, float Kd);
        int setP(float dt, float max, float min);
        float calc(float currentValue, float desiredValue);
        void displayInfo();

    private:
        float _Kp;
        float _Ki;
        float _Kd;
        float _dt = 0.1;
        float _max = 1.0;
        float _min = -1.0;
        float _pre_error = 0.0;
        float _sum_error = 0.0;
        float _maxVal = 180.0;
        float _minVal = 0.0;
};

#endif