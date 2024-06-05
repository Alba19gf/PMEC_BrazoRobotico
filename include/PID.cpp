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

int PID::setWindup(float Windup)
{
    _Windup = Windup;
    return 0;
}

int PID::reset()
{
    _pre_error = 0.0;
    _sum_error = 0.0;
    return 0;
}

float PID::calc(float currentValue, float desiredValue)
{
    if(desiredValue > _maxRange)
    {
        Serial.printf("[ERROR]: Valor deseado fuera de rango... máx: %f, desVal:%f \n", _maxRange, desiredValue);
        return 0;
    }

    if(desiredValue < _minRange)
    {
        Serial.printf("[ERROR]: Valor deseado fuera de rango... mín: %f, desVal:%f \n", _minRange, desiredValue);
        return 0;
    }
        
    _error = desiredValue - currentValue;

    __P = _Kp * _error;

    // Windup??? TODO: Max Windup and Min Windup
    // TODO: Check if this is the correct way to implement Windup
    // Windup para parte integral
    if(abs((_Ki * _sum_error)) < _Windup && _Ki > 0)
    {
        _sum_error += _error * _dt;
    }    

    _I = _Ki * _sum_error;
    // Parte integrativa mismo sentido que el error
    if(_P < 0 && _I > 0 || __P > 0 && _I < 0)
    {
        _I = -_I;
    }
    
    _D = (_Kd/_dt) * (_error - _pre_error);
    
    _PID = __P + _I + _D;

    // Windup para PID (de -1 a 1 según el dutycycle)
    if (_PID <= _min)
        _PID = _min;
    if (_PID >= _max)
        _PID = _max;

    _pre_error = _error;

    if(DEBUG & DEBUG_PID != 0)
    {
        if(DEBUG & DEBUG_PLOT != 0)
        {
            /*Serial.printf(">desPos: %f\n", desiredValue);
            Serial.printf(">currPos: %f\n", currentValue);
            Serial.printf(">error: %f\n", error);
            
            Serial.printf(">Error: %f\n", error);
            Serial.printf(">_pre_error: %f\n", _pre_error);
            Serial.printf(">sum_error: %f\n", _sum_error);
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

    return _PID;
}

void PID::displayInfo()
{
    //Serial.printf("Kp: %f, Ki: %f, Kd: %f, dt: %f, max: %f, min: %f\n", _Kp, _Ki, _Kd, _dt, _max, _min);
    Serial.printf("Kp: %f, Ki: %f, Kd: %f\n", _Kp, _Ki, _Kd);
    Serial.printf("_Windup: %f \n", _Windup); 
}