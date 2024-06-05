#include <Arduino.h>
#include <SPI.h>
#include "../include/Almar.h"
//#include "../include/FnKinematics/FnKinematics.cpp"

hw_timer_t *timer = NULL;

int ctrl = false;
void IRAM_ATTR timerCtrl() {
 ctrl = true;
}

const float dt = 0.1;

float tol = 0.5;
float error_global[N_MOTORS];
float error_local;

void setup() {
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
  cmd.AddCommand(&cmd_set_encoder_zero_);
  cmd.AddCommand(&cmd_set_motor_kp_);
  cmd.AddCommand(&cmd_set_motor_ki_);
  cmd.AddCommand(&cmd_set_motor_kd_);
  cmd.AddCommand(&cmd_get_encoder_deg_);
  cmd.AddCommand(&cmd_get_motor_info_);

  // Inicializamos el timer
  timer = timerBegin(0, 80, true);                // Timer 0, clock divider 80
  timerAttachInterrupt(timer, &timerCtrl, true);  // Attach the interrupt handling function
  timerAlarmWrite(timer, 1000000*dt, true);          // Interrupt every 1 second
  timerAlarmEnable(timer);                        // Enable the alarm


  if(DEBUG & DEBUG_INFO != 0)
    Serial.printf("Serial ready!\n");

  if(DEBUG & DEBUG_EXTRA != 0)
    Serial.printf("DEBUG MODE: %i\n", DEBUG);

  // LEDS
  pinMode(led_1, OUTPUT);
  pinMode(led_2, OUTPUT);

  analogWrite(led_1, 0);
  analogWrite(led_2, 0);
  
  // Recorremos todos los motores existe en el array motor
  for(int i = 0; i < N_MOTORS; i++)
  {
    // Inicializamos el motor
    _m[i] = new AlMar::Esp32::Driver_L298n(motor[i][0], motor[i][1], motor[i][2], motor[i][3]);
    _m[i]->begin();
    // Inicializamos el control
    _pid[i] = new PID(motor[i][4], motor[i][5], motor[i][6]);
    _pid[i]->setRange(motor[i][7], motor[i][8]);
    _pid[i]->setWindup(motor[i][9]);
  }
  // Inicializamos los encoders
  _enc = new AlMar::Esp32::EncoderATM203_Spi2(cs_pins,N_MOTORS,PIN_MOSI,PIN_MISO,PIN_SCLK);
}

void loop() {
  cmd.ReadSerial();

  if(ctrl)
  {
    // Lectura de los encoders (0, 1, 2
    pos[0] = (float) _enc->Read(0)*(360.0/4096.0);
    pos[1] = (float) _enc->Read(1)*(360.0/4096.0);
    pos[2] = (float) _enc->Read(2)*(360.0/4096.0);

    if(pos[0] < 360 && pos[0] > -360)
    {
      if((pos[0] - pastPos[0]) < -180)
        pos[0] += 360;
      else if((pos[0] - pastPos[0]) > 180)
        pos[0] -= 360;

      error_global[0] = abs(pos[0] - desPos[0]);
      
      if(error_global[0] > tol)
      {
        dutyCycle[0] = _pid[0]->calc(pos[0], desPos[0]);
        _m[0]->SetDuty(dutyCycle[0]);
      }
      else
      {
        _m[0]->SetDuty(0.05);
        _pid[0]->reset();
      }
    }

    if(pos[1] < 360 && pos[1] > -360)
    {
      if((pos[1] - pastPos[1]) < -180)
        pos[1] += 360;
      else if((pos[1] - pastPos[1]) > 180)
        pos[1] -= 360;

      error_global[1] = abs(pos[1] - desPos[1]);
      
      if(error_global[1] > tol)
      {

        dutyCycle[1] = _pid[1]->calc(pos[1], desPos[1]);
        _m[1]->SetDuty(dutyCycle[1]);
      }
      else
      {
        _m[1]->SetDuty(0.05);
        _pid[1]->reset();
      }
    }

    if(pos[2] < 360 && pos[2] > -360)
    {
      if((pos[2] - pastPos[2]) < -180)
        pos[2] += 360;
      else if((pos[2] - pastPos[2]) > 180)
        pos[2] -= 360;

      error_global[2] = abs(pos[2] - desPos[2]);
      
      if(error_global[2] > tol)
      {
        dutyCycle[2] = _pid[2]->calc(pos[2], desPos[2]);
        _m[2]->SetDuty(dutyCycle[2]);
      }
      else
      {
        _m[2]->SetDuty(0.05);
        _pid[2]->reset();
      }
    }

    Serial.printf(">pos[0]: %f\n", pos[0]);
    Serial.printf(">desPos[0]: %f\n", desPos[0]);
    Serial.printf(">error_global[0]: %f\n", error_global[0]);
    Serial.printf(">error[0]: %f\n", _pid[0]->_error);
    Serial.printf(">P[0]:%f\n", _pid[0]->__P);
    Serial.printf(">I[0]:%f\n", _pid[0]->_I);
    Serial.printf(">D[0]:%f\n", _pid[0]->_D);
    Serial.printf(">PID[0]:%f\n", _pid[0]->_PID);

    Serial.printf(">pos[1]: %f\n", pos[1]);
    Serial.printf(">desPos[1]: %f\n", desPos[1]);
    Serial.printf(">error_global[1]: %f\n", error_global[1]);
    Serial.printf(">error[1]: %f\n", _pid[1]->_error);
    Serial.printf(">P[1]:%f\n", _pid[1]->__P);
    Serial.printf(">I[1]:%f\n", _pid[1]->_I);
    Serial.printf(">D[1]:%f\n", _pid[1]->_D);
    Serial.printf(">PID[1]:%f\n", _pid[1]->_PID);

    Serial.printf(">pos[2]: %f\n", pos[2]);
    Serial.printf(">desPos[2]: %f\n", desPos[2]);
    Serial.printf(">error_global[2]: %f\n", error_global[2]);
    Serial.printf(">error[2]: %f\n", _pid[2]->_error);
    Serial.printf(">P[2]:%f\n", _pid[2]->__P);
    Serial.printf(">I[2]:%f\n", _pid[2]->_I);
    Serial.printf(">D[2]:%f\n", _pid[2]->_D);
    Serial.printf(">PID: %f\n", _pid[2]->_PID);

    pastPos[0] = pos[0];
    pastPos[1] = pos[1];
    pastPos[2] = pos[2];

    ctrl = false;
  }
}

  /*if(ctrl)
  {
    pos[0] = (float) _enc->Read(0)*(360.0/4096.0);
    pos[1] = (float) _enc->Read(0)*(360.0/4096.0);
    pos[2] = (float) _enc->Read(0)*(360.0/4096.0);

    TCP_d = arrayTLineal(pos[0], 250, -140, desPos[0], 250, -140); // TODO: Update x init

    error_global = abs(pos[0] - TCP_d[num_pasos-1][0]);

    Serial.printf(">pos[0]: %f\n", pos[0]);
    Serial.printf(">desPos[0]: %f\n", desPos[0]);
    Serial.printf(">TCP_d[num_pasos-1][0]: %f\n", TCP_d[num_pasos-1][0]);
    Serial.printf(">error_global: %f\n", error_global);
    Serial.printf(">dutyCycle: %f\n", dutyCycle);

    if(pos[0]!=0x80000000 || pos[1]!=0x80000000 || pos[2]!=0x80000000) { 
      
      //Serial.printf("Posición inicial: %f \t Posición final: %f \n", pos[0], TCP_d[num_pasos-1][0]);

      if(error_global > tol)
      {
        for(int i = 0; i < num_pasos; i++)
        {
          desPos[0] = TCP_d[i][0];
          //Serial.printf("[%i] Posición actual: %f \t Posición deseada: %f \n", i, pos[0], desPos);

          error_local = abs(pos[0] - desPos[0]);

          while(error_local > tol)
          {
            pos[0] = (float) (_enc->Read(0)*360.0/4096.0);
            dutyCycle = _pid[0]->calc(pos[0], desPos[0]);
            _m[0]->SetDuty(dutyCycle);
          }
        }
      }
      else
      {
        _m[0]->SetDuty(0);
      }
      
    }

    ctrl = false;
  }*/
