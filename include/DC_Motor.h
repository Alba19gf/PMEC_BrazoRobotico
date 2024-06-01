#include <Arduino.h>

#ifndef DC_MOTOR_H
#define DC_MOTOR_H

/// @brief Clase para gestionar motores DC utilizando la librería de Arduino UNO. Complatible con ESP32-S3-DEVKITC-1 (WROOM-2).
class DC_Motor 
{
    public:
        /// @brief                  Clase constructora para crear el objeto motor.
        /// @param motor_pin_1      Número de PIN en el ESP32 que para la salida de la señal PWM 1.
        /// @param motor_pin_2      Número de PIN en el ESP32 que para la salida de la señal PWM 2.
        /// @param channel          Canal al cuál se van a añadir los pines para la señal PWM. 0 por defecto.
        /// @param frequency        Frecuencia a la cuál se estará trabajando. Mínimo 20,000. 50,000 por defecto.
        /// @param resolution       Reslución a la que se trabajará. 8 bits por defecto.
        DC_Motor(int motor_pin_1, int motor_pin_2, int channel = 0, int frequency = 50000, int resolution = 8){}

    private:
        int motor_pin_1;
        int motor_pin_2;
        int channel;
        int frequency;
        int resolution;
};

#endif