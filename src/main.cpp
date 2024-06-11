#include <Arduino.h>
#include <SPI.h>
#include "../include/Almar.h"

const float dt = 0.1;

float tol_global = 0.6;
float tol_local = 0.2;
float tol_tcp = 0.5;

float error_global[N_MOTORS] = {1, 1, 1};
float error_local[N_MOTORS] = {1, 1, 1};

// Base motor 0
// Shoulder motor 1
// Elbow motor 2
int p = 0; // step
int q = 0; // step
int t = 0; // step

float* steps_x;
int n_steps_x;
float* steps_y;
int n_steps_y;
float* steps_z;
int n_steps_z;
int num_pasosv2;

int init_ctrl = 0;
int init_pos = 0;

float pastTcp[3] = {0, 0, 0};
float origin[3] = {0, 0, 0};

hw_timer_t *timer = NULL;
int ctrl = false;
void IRAM_ATTR timerCtrl() {
 ctrl = true;
 // Lectura de los encoders (0, 1, 2)
    pos[0] = (float) _enc->Read(0)*(360.0/4096.0);
    pos[1] = (float) _enc->Read(1)*(360.0/4096.0);      
    pos[2] = (float) _enc->Read(2)*(360.0/4096.0);

    // Corrección 360 grados
    if((pos[0] - pastPos[0]) < -180)
          pos[0] += 360;
    else if((pos[0] - pastPos[0]) > 180)
          pos[0] -= 360;

    if((pos[1] - pastPos[1]) < -180)
          pos[1] += 360;
    else if((pos[1] - pastPos[1]) > 180)
          pos[1] -= 360;

    if((pos[2] - pastPos[2]) < -180)
          pos[2] += 360;
    else if((pos[2] - pastPos[2]) > 180)
          pos[2] -= 360;
    
    pastPos[0] = pos[0];
    pastPos[1] = pos[1];
    pastPos[2] = pos[2];
    
    // Corrección direcciones
    pos[1] = -pos[1];
    pos[2] = -pos[2];
    // Corrección relación engranajes
    //pos[1] = pos[1]*6/7;
    pos[2] = pos[2]*11/18;
    // Corrección origen
    pos[0] += origin[0];
    pos[1] += origin[1];
    pos[2] += origin[2];

    nums curr_tcp = FnDirKinem(pos[0], pos[1], pos[2]);
    tcp[0] = curr_tcp.uno;
    tcp[1] = curr_tcp.dos;
    tcp[2] = curr_tcp.tres;

    if(pastTcp[0] != des_tcp[0] || pastTcp[1] != des_tcp[1] || pastTcp[2] != des_tcp[2] || init_ctrl)
    {
      struct nums angles1 = FnInvKinem(tcp[0], tcp[1], tcp[2]);
      struct nums angles2 = FnInvKinem(des_tcp[0], des_tcp[1], des_tcp[2]);
      int num_pasos_x = ceil(abs(angles1.uno - angles2.uno)/paso_x);
      int num_pasos_y = ceil(abs(angles1.dos - angles2.dos)/paso_y);
      int num_pasos_z = ceil(abs(angles1.tres - angles2.tres)/paso_z);

      num_pasosv2 = MAX(num_pasos_x, num_pasos_y);
      num_pasosv2 = MAX(num_pasosv2, num_pasos_z);
      n_steps_x = num_pasosv2;
      n_steps_y = num_pasosv2;
      n_steps_z = num_pasosv2;

      steps_x = tLineal_x(tcp[0], tcp[1], tcp[2], des_tcp[0], des_tcp[1], des_tcp[2], num_pasosv2);
      steps_y = tLineal_y(tcp[0], tcp[1], tcp[2], des_tcp[0], des_tcp[1], des_tcp[2], num_pasosv2);
      steps_z = tLineal_z(tcp[0], tcp[1], tcp[2], des_tcp[0], des_tcp[1], des_tcp[2], num_pasosv2);
      // Motor 0
      _pid[0]->reset(); 
      p = 0;
      // Motor 1
      _pid[1]->reset(); 
      q = 0;
      // Motor 2
      _pid[2]->reset();
      t = 0;

      
    end_ctrl = 0;
    }
    
    pastTcp[0] = des_tcp[0];
    pastTcp[1] = des_tcp[1];
    pastTcp[2] = des_tcp[2];

    // Motor 0
    desPos[0] = steps_x[p];
    bool hasOvershot = false;

      if(steps_x[p] < steps_x[p+1]) {
              // Moving in positive direction
              if(pos[0] > steps_x[p+1]) {
                  hasOvershot = true;
              }
          } else if(steps_x[p] > steps_x[p+1]) {
              // Moving in negative direction
              if(pos[0] < steps_x[p+1]) {
                  hasOvershot = true;
              }
          }
      
      error_local[0] = abs(pos[0] - steps_x[p]);
      if(error_local[0] < tol_local || hasOvershot)
      {
          p++;
          if(p == n_steps_x)
          {
            p = n_steps_x-1;
            _pid[0]->reset();
          }
      }
      
      dutyCycle[0] = _pid[0]->calc(pos[0], steps_x[p]);
      _m[0]->SetDuty(dutyCycle[0]);

    // Motor 1
    desPos[1] = steps_y[q];
    
    hasOvershot = false;

      if(steps_y[q] < steps_y[q+1]) {
              // Moving in positive direction
              if(pos[1] > steps_y[q+1]) {
                  hasOvershot = true;
              }
          } else if(steps_y[q] > steps_y[q+1]) {
              // Moving in negative direction
              if(pos[1] < steps_y[q+1]) {
                  hasOvershot = true;
              }
          }
      
      error_local[1] = abs(pos[1] - steps_y[q]);
      if(error_local[1] < tol_local || hasOvershot)
      {
          q++;
          if(q == n_steps_y)
          {
            q = n_steps_y-1;
            _pid[1]->reset();
          }
      }
      
      dutyCycle[1] = _pid[1]->calc(pos[1], steps_y[q]);
      _m[1]->SetDuty(dutyCycle[1]);
            
    // Motor 2
    desPos[2] = steps_z[t];

    hasOvershot = false;

      if(steps_z[t] < steps_z[t+1]) {
              // Moving in positive direction
              if(pos[2] > steps_z[t+1]) {
                  hasOvershot = true;
              }
          } else if(steps_z[t] > steps_z[t+1]) {
              // Moving in negative direction
              if(pos[2] < steps_z[t+1]) {
                  hasOvershot = true;
              }
          }
      
      error_local[2] = abs(pos[2] - steps_z[t]);
      if(error_local[2] < tol_local || hasOvershot)
      {
          t++;
          if(t == n_steps_z)
          {
            t = n_steps_z-1;
            _pid[2]->reset();
          }
      }
      
      dutyCycle[2] = _pid[2]->calc(pos[2], steps_z[t]);
      _m[2]->SetDuty(dutyCycle[2]);

      if(counter == 11)
      {
        counter = 0;
      }
      else
      {
        counter++;
      }
      
}

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
  // Motores
  cmd.AddCommand(&cmd_set_motor_pos_);
  cmd.AddCommand(&cmd_set_encoder_zero_);
  cmd.AddCommand(&cmd_set_motor_kp_);
  cmd.AddCommand(&cmd_set_motor_ki_);
  cmd.AddCommand(&cmd_set_motor_kd_);
  cmd.AddCommand(&cmd_get_encoder_deg_);
  cmd.AddCommand(&cmd_get_encoders_deg_);
  cmd.AddCommand(&cmd_get_motor_info_);
  // TCP
  cmd.AddCommand(&cmd_set_tcp_);
  cmd.AddCommand(&cmd_get_tcp_);
  cmd.AddCommand(&cmd_goto_);
  cmd.AddCommand(&cmd_goZ_);
  cmd.AddCommand(&cmd_goSafe_);
  cmd.AddCommand(&cmd_goHome_);
  // Gripper
  cmd.AddCommand(&cmd_openGripper_);
  cmd.AddCommand(&cmd_closeGripper_);

  cmd.AddCommand(&cmd_moverPieza_);

  motorPinza.attach(MOTOR_PINZA_PIN);

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

  // Calculamos la corrección del error de los encoders para la posición 5
  nums temp = FnInvKinem(0, 330, -145);
  origin[0] = temp.uno;
  origin[1] = temp.dos;
  origin[2] = temp.tres;

  pos[0] = (float) _enc->Read(0)*(360.0/4096.0);
  pos[1] = (float) _enc->Read(1)*(360.0/4096.0);      
  pos[2] = (float) _enc->Read(2)*(360.0/4096.0);

  nums curr_tcp = FnDirKinem(pos[0], pos[1], pos[2]);
  tcp[0] = curr_tcp.uno;
  tcp[1] = curr_tcp.dos;
  tcp[2] = curr_tcp.tres;

  des_tcp[0] = tcp[0];
  des_tcp[1] = tcp[1];
  des_tcp[2] = tcp[2];

  pastTcp[0] = des_tcp[0];
  pastTcp[1] = des_tcp[1];
  pastTcp[2] = des_tcp[2];
  
  init_ctrl = 1;
}

void loop() {
  cmd.ReadSerial();
  
  if(ctrl)
  {
    
    Serial.printf(">estado: %i\n", estado);
    Serial.printf(">n_steps_x: %i\n", n_steps_x-1);
    Serial.printf(">n_steps_y: %i\n", n_steps_y-1);
    Serial.printf(">n_steps_z: %i\n", n_steps_z-1);
    Serial.printf(">p: %i\n", p);
    Serial.printf(">q: %i\n", q);
    Serial.printf(">t: %i\n", t);


    for(int i = 0; i < N_MOTORS; i++)
    {
      if(i == 0 || i == 1 || i == 2)
      {
        Serial.printf(">pos[%i]: %f\n", i, pos[i]);
        Serial.printf(">desPos[%i]: %f\n", i, desPos[i]);
        Serial.printf(">error_global[%i]: %f\n", i, error_global[i]);
        Serial.printf(">error_local[%i]: %f\n", i, error_local[i]);
        Serial.printf(">error[%i]: %f\n", i, _pid[i]->_error);
        Serial.printf(">P[%i]:%f\n", i, _pid[i]->__P);
        Serial.printf(">I[%i]:%f\n", i, _pid[i]->_I);
        Serial.printf(">D[%i]:%f\n", i, _pid[i]->_D);
        Serial.printf(">PID[%i]:%f\n", i, _pid[i]->_PID);
      }
    }

    Serial.printf(">tcp[x]: %f\n", tcp[0]);
    Serial.printf(">tcp[y]: %f\n", tcp[1]);
    Serial.printf(">tcp[z]: %f\n", tcp[2]);
    Serial.printf(">des_tcp[x]: %f\n", des_tcp[0]);
    Serial.printf(">des_tcp[y]: %f\n", des_tcp[1]);
    Serial.printf(">des_tcp[z]: %f\n", des_tcp[2]);

    /*while(p > 0)
    {
      end_ctrl = false;
    }
    end_ctrl = true;*/

    if(p == n_steps_x-1 && q == n_steps_y-1 && t == n_steps_z-1)
    {
      end_ctrl = 1;
    }

    Serial.printf(">end_ctrl: %i\n", end_ctrl);
    ctrl = false;
    moverPieza(ficha, posicion);
    Serial.printf(">ficha: %i\n", ficha);
    Serial.printf(">posicion: %i\n", posicion);
  }


}
  