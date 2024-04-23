#include "Arduino.h"
#include "PID.h"

PID::PID()
{
    PID::setK(-1.0, -2.0, -3.0);
    PID::setP(1.0, 2.0, 3.0);
    Serial.printf("1st constructor... \n");
}

PID::PID(float Kp, float Ki, float Kd, float dt, float max, float min)
{
    PID::setK(Kp, Ki, Kd);
    PID::setP(dt, max, min);
    Serial.printf("PID initialized with 2nd constructor... \n");
    PID::displayInfo();
}

int PID::setK(float Kp, float Ki, float Kd)
{
    _Kp = Kp;
    _Ki = Ki;
    _Kd = Kd;
    return 0;
}

int PID::setP(float dt, float max, float min)
{
    _dt = dt;
    _max = max;
    _min = min;
    return 0;
}

float PID::calc(float currentValue, float desiredValue)
{
    float error = desiredValue - currentValue;

    float P = _Kp * error;

    _sum_error += error * _dt;
    float I = _Ki * _sum_error;
    
    float D = (_Kd/_dt) * (error - _pre_error);

    float PID = P + I + D;

    if (PID <= _min)
        PID = _min;
    if (PID >= _max)
        PID = _max;

    _pre_error = error;

    Serial.printf("PID: %f, P: %f, I: %f, D:%f \t###", PID, P, I, D);
    Serial.printf("desPos: %f, currPos: %f, error: %f, _pre_error: %f\n", desiredValue, currentValue, error, _pre_error);

    return PID;
}

void PID::displayInfo()
{
    Serial.printf("Kp: %f, Ki: %f, Kd: %f, dt: %f, max: %f, min: %f\n", _Kp, _Ki, _Kd, _dt, _max, _min);
}