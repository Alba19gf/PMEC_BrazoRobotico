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
        int setRange(float maxRange, float minRange);
        int setWindup(float Windup);
        int reset();
        float calc(float currentValue, float desiredValue);
        void displayInfo();

    public:
        float _Kp;
        float _Ki;
        float _Kd;
        float _dt = 0.1;
        float _max = 1.0;
        float _min = -1.0;
        float _pre_error = 0.0;
        float _sum_error = 0.0;
        float _maxRange = 180.0;
        float _minRange = 0.0;
        float _Windup = 0.4;

        float _error = 0.0;
        float __P = 0.0;
        float _I = 0.0;
        float _D = 0.0;
        float _PID = 0.0;

};

#endif