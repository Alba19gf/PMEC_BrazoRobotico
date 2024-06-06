#include <Arduino.h>
#include <SPI.h>
#include "../include/Almar.h"
//#include "../include/FnKinematics/FnKinematics.cpp"

hw_timer_t *timer = NULL;


int ctrl = false;
void IRAM_ATTR timerCtrl() {
 ctrl = true;
}

const float dt = 0.05;

float tol_global = 0.5;
float tol_local = 0.1;
float error_global[N_MOTORS];
float error_local[N_MOTORS];

double pasos[N_MOTORS][5] = {
                          {-20, -20+1.3*1, -20+1.3*2, -20+1.3*3, -20+1.3*4},
                          {0+0.9*0, 0+0.9*1, 0+0.9*2, 0+0.9*3, 0+0.9*4},
                          {0+1.16*0, 0+1.16*1, 0+1.16*2, 0+1.16*3, 0+1.16*4}
    };

int i = 0;  // motor
int j = 0; // step

void setup() {
   // Inicializamos el timer
  timer = timerBegin(0, 80, true);                // Timer 0, clock divider 80
  timerAttachInterrupt(timer, &timerCtrl, true);  // Attach the interrupt handling function
  timerAlarmWrite(timer, 1000000*dt, true);       // Interrupt every 1 second
  timerAlarmEnable(timer);                        // Enable the alarm

  // Inicializamos el puerto serial
  Serial.begin(115200);
  // Inicializamos el puerto serial para los comandos
  // Comando por defecto para comandos no reconocidos
  cmd.SetDefaultHandler(cmd_unrecognized);
  // Registramos los comandos para el puerto serial abierto
  cmd.AddCommand(&cmd_set_debug_);
  cmd.AddCommand(&cmd_set_led1_);
  cmd.AddCommand(&cmd_set_led2_);
  cmd.AddCommand(&cmd_set_motor_pos_);
  cmd.AddCommand(&cmd_set_pos_);
  cmd.AddCommand(&cmd_set_encoder_zero_);
  cmd.AddCommand(&cmd_set_motor_kp_);
  cmd.AddCommand(&cmd_set_motor_ki_);
  cmd.AddCommand(&cmd_set_motor_kd_);
  cmd.AddCommand(&cmd_get_encoder_deg_);
  cmd.AddCommand(&cmd_get_encoders_deg_);
  cmd.AddCommand(&cmd_get_motor_info_);

  if(DEBUG & DEBUG_INFO != 0)
    Serial.printf("Serial ready!\n");

  if(DEBUG & DEBUG_EXTRA != 0)
    Serial.printf("DEBUG MODE: %i\n", DEBUG);

  // LEDS
  pinMode(led_1, OUTPUT);
  pinMode(led_2, OUTPUT);

  analogWrite(led_1, 0);
  analogWrite(led_2, 0);
  
  // Inicializamos los encoders
  _enc = new AlMar::Esp32::EncoderATM203_Spi2(cs_pins,N_MOTORS,PIN_MOSI,PIN_MISO,PIN_SCLK);
  
  // Recorremos todos los motores existe en el array motor
  for(int i = 0; i < N_MOTORS; i++)
  {
    // Inicializamos el motor
    _m[i] = new AlMar::Esp32::Driver_L298n(motor[i][0], motor[i][1], motor[i][2], motor[i][3]);
    _m[i]->begin();
    // Inicializamos el control
    _pid[i] = new PID(motor[i][4], motor[i][5], motor[i][6]);
    _pid[i]->setdt(dt);
    _pid[i]->setRange(motor[i][7], motor[i][8]);
    _pid[i]->setWindup(motor[i][9]);

    // Primera posición anterior para inicio fuera de origen
    //pastPos[i] = (float) _enc->Read(i)*(360.0/4096.0);
  }
}

void loop() {
  cmd.ReadSerial();

  if(ctrl)
  {
    // Lectura de los encoders (0, 1, 2)
    pos[0] = (float) _enc->Read(0)*(360.0/4096.0);
    pos[1] = (float) _enc->Read(1)*(360.0/4096.0);
    pos[2] = (float) _enc->Read(2)*(360.0/4096.0);

    desPos[i] = pasos[i][j];
    desPos[i+1] = pasos[i+1][j];
    desPos[i+2] = pasos[i+2][j];

    if((pos[i] - pastPos[i]) < -180)
          pos[i] += 360;
    else if((pos[0] - pastPos[i]) > 180)
          pos[i] -= 360;

    // Calculamos el error global
    error_global[i] = abs(pos[i] - pasos[i][4]);

    // Comprobamos si hemos llegado a la posición deseada final
    if(error_global[i] > tol_global)
    {
      // Calculamos el error local
      error_local[i] = abs(pos[i] - pasos[i][j]);

      dutyCycle[i] = _pid[i]->calc(pos[i], desPos[i]);
      _m[i]->SetDuty(dutyCycle[i]);

      //if(error_local[i] < tol_local)
      if(error_local[i] < tol_local && pos[i] < pasos[i][j+1])
      {
        // Cambiamos de paso
        j++;
        if(j > 4)
          j = 4;

        _pid[i]->reset();
        Serial.printf("desPos[%i]: %f\n", i, desPos[i]);
        Serial.printf("pasos[%i][%i]: %f\n", i, j, pasos[i][j]);
      }
    }
    else
    {
      _m[i]->SetDuty(0.05);
      _pid[i]->reset();
    }

    // Actualizamos la última posición
    pastPos[i] = pos[i];

    // DEBUG
    Serial.printf(">pos[0]: %f\n", pos[0]);
    Serial.printf(">desPos[0]: %f\n", desPos[0]);
    Serial.printf(">error_global[0]: %f\n", error_global[0]);
    Serial.printf(">error_local[0]: %f\n", error_local[0]);
    Serial.printf(">error[0]: %f\n", _pid[0]->_error);
    Serial.printf(">P[0]:%f\n", _pid[0]->__P);
    Serial.printf(">I[0]:%f\n", _pid[0]->_I);
    Serial.printf(">D[0]:%f\n", _pid[0]->_D);
    Serial.printf(">PID[0]:%f\n", _pid[0]->_PID);

    /*Serial.printf(">pos[1]: %f\n", pos[1]);
    Serial.printf(">desPos[1]: %f\n", desPos[1]);
    Serial.printf(">error_global[1]: %f\n", error_global[1]);
    Serial.printf(">error_local[1]: %f\n", error_local[1]);
    Serial.printf(">error[1]: %f\n", _pid[1]->_error);
    Serial.printf(">P[1]:%f\n", _pid[1]->__P);
    Serial.printf(">I[1]:%f\n", _pid[1]->_I);
    Serial.printf(">D[1]:%f\n", _pid[1]->_D);
    Serial.printf(">PID[1]:%f\n", _pid[1]->_PID);

    Serial.printf(">pos[2]: %f\n", pos[2]);
    Serial.printf(">desPos[2]: %f\n", desPos[2]);
    Serial.printf(">error_global[2]: %f\n", error_global[2]);
    Serial.printf(">error_local[2]: %f\n", error_local[2]);
    Serial.printf(">error[2]: %f\n", _pid[2]->_error);
    Serial.printf(">P[2]:%f\n", _pid[2]->__P);
    Serial.printf(">I[2]:%f\n", _pid[2]->_I);
    Serial.printf(">D[2]:%f\n", _pid[2]->_D);
    Serial.printf(">PID[2]: %f\n", _pid[2]->_PID);*/

    ctrl = false;
  }
}

  