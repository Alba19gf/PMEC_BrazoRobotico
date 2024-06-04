#include "Arduino.h"
#include "PID.h"

PID::PID()
{
    PID::setK(-1.0, -2.0, -3.0);
    PID::setP(1.0, 2.0, 3.0);
}

PID::PID(float Kp, float Ki, float Kd)
{
    PID::setK(Kp, Ki, Kd);
}

PID::PID(float Kp, float Ki, float Kd, float dt, float max, float min)
{
    PID::setK(Kp, Ki, Kd);
    PID::setP(dt, max, min);
}

int PID::setKP(float Kp)
{
    _Kp = Kp;
    return 0;
}

int PID::setKI(float Ki)
{
    _Ki = Ki;
    return 0;
}

int PID::setKD(float Kd)
{
    _Kd = Kd;
    return 0;
}

int PID::setdt(float dt)
{
    _dt = dt;
    return 0;
}

int PID::setMax(float max)
{
    _max = max;
    return 0;
}

int PID::setMin(float min)
{
    _min = min;
    return 0;
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

int PID::setRange(float maxRange, float minRange)
{
    _maxRange = maxRange;
    _minRange = minRange;
    return 0;
}

float PID::calc(float currentValue, float desiredValue)
{
    if(desiredValue > _maxRange)
        Serial.printf("[ERROR]: Valor deseado fuera de rango... máx: %f, desVal:%f \n", _maxRange, desiredValue);
    if(desiredValue < _minRange)
        Serial.printf("[ERROR]: Valor deseado fuera de rango... mín: %f, desVal:%f \n", _minRange, desiredValue);
        
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

    if(DEBUG & DEBUG_PID != 0)
    {
        if(DEBUG & DEBUG_PLOT != 0)
        {
            /*Serial.printf(">desPos: %f\n", desiredValue);
            Serial.printf(">currPos: %f\n", currentValue);
            Serial.printf(">error: %f\n", error);

            Serial.printf(">PID: %f\n", PID);
            Serial.printf(">P: %f\n", P);
            Serial.printf(">I: %f\n", I);
            Serial.printf(">D: %f\n", D);*/
        }

        if(DEBUG & DEBUG_INFO != 0)
        {
            //Serial.printf("PID: %f, P: %f, I: %f, D:%f \n", PID, P, I, D);
            //Serial.printf("desPos: %f, currPos: %f, error: %f, _pre_error: %f\n", desiredValue, currentValue, error, _pre_error);
        }
    }

    return PID;
}

void PID::displayInfo()
{
    Serial.printf("Kp: %f, Ki: %f, Kd: %f, dt: %f, max: %f, min: %f\n", _Kp, _Ki, _Kd, _dt, _max, _min);
}