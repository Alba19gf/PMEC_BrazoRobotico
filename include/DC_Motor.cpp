#include <DC_Motor.h>
#include <stdexcept>

class DC_Motor
{
    public:
        DC_Motor(int motor_pin_1, int motor_pin_2, int channel = 0, int frequency = 50000, int resolution = 8)
        {
            if(motor_pin_1 < 0 || motor_pin_2 < 0 || channel < 0 || frequency < 0 || resolution < 0)
                throw std::invalid_argument("[DC_MOTOR]: Ninguno de los argumentos puede ser negativo");
        }

    private:
        int motor_pin_1;
        int motor_pin_2;
        int channel;
        int frequency;
        int resolution;
};