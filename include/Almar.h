#include <Arduino.h>
#include <ESP32Servo.h>

// Constante para mensajes de debug 
#define DEBUG_INFO    1
#define DEBUG_WARNING 2
#define DEBUG_ERROR   4
#define DEBUG_PLOT    8
#define DEBUG_EXTRA   16
#define DEBUG_PID     32
#define DEBUG_ALL     DEBUG_INFO | DEBUG_WARNING | DEBUG_ERROR | DEBUG_PLOT | DEBUG_EXTRA | DEBUG_PID
int DEBUG = DEBUG_ALL;

#include "../lib/Almar/ALMar_ESP32_Driver_L298n.cpp"
#include "../lib/Almar/ALMar_ESP32_EncoderATM203_Spi2.cpp"
#include "../include/PID.cpp"
#include "../lib/Serial/SerialCommands.h"
#include "../include/FnKinematics/FnKinematics.cpp"
#include "../include/FnKinematics/FnKinematicsv2.cpp"

#define PIN_CS_M1  1 //(chip select para encoder de motor 1) 10 for SPI3
#define PIN_CS_M2  2 //(chip select para encoder de motor 2) 
#define PIN_CS_M3  3 //(chip select para encoder de motor 3) 
// Pines comunes a todos los encóders:
#define PIN_SCLK  18 // CLOCK
#define PIN_MISO  19 // MISO
#define PIN_MOSI  13 // MOSI

// Led
const int led_1 = 40;
const int led_2 = 41;

// Gripper
#define MOTOR_PINZA_PIN 11
Servo motorPinza;


// CONFIGURACIÓN MOTORES
// Pines motores (EN, IN1, IN2, PWM)
// Constantes control (KP, KI, KD)
// Rango control (max, min, windup)
double motor[3][10] = { 
                      /*{0, 5, 4, 25000, 0.27, 0.05, 0.0005, 360, -360, 0.625},         // Motor 1 con 500g
                      {45, 7, 6, 25000, 0.3, 0.00, 0.00, 360, -360, 0.625},      // Motor 2
                      {39, 10, 9, 25000, 0.2, 0.00, 0.00, 360, -360, 0.6}      // Motor 3*/
                      {0, 5, 4, 25000, 0.3, 0.1, 0.00, 360, -360, 0.425},         // Motor 1 con 500g
                      {45, 7, 6, 25000, 0.45, 0.2, 0.00, 360, -360, 0.6},      // Motor 2
                      {39, 10, 9, 25000, 0.4, 0.2, 0.00, 360, -360, 0.6}      // Motor 3
                    };
// Cantidad motores
const int N_MOTORS = sizeof(motor)/sizeof(motor[0]);

// Encoder
int cs_pins[]={PIN_CS_M1, PIN_CS_M2, PIN_CS_M3};
int pwm_freq = 25000;

int counter = 0;
int end_ctrl = 0;
int ficha = -1;
int posicion = -1;
int estado = 0;

// CONTROL
float dutyCycle[N_MOTORS] = {};

// Control motores
float desPos[N_MOTORS] = {0, 0, 0};
float pos[N_MOTORS] = {};
float pastPos[N_MOTORS] = {0,0,0};

// Control TCP
float tcp[N_MOTORS];
float des_tcp[N_MOTORS];

// Posiciones tablero
float _goZ = -145;
float _goSafe = -80;

float c9[3] = {40.0,  369.0, _goSafe};
float c8[3] = { 0.0,  369.0, _goSafe};
float c7[3] = {-40.0, 369.0, _goSafe};
float c6[3] = {40.0,  335.0, _goSafe};
float c5[3] = { 0.0,  335.0, _goSafe};
float c4[3] = {-40.0, 335.0, _goSafe};
float c3[3] = {40.0,  305.0, _goSafe};
float c2[3] = { 0.0,  305.0, _goSafe};
float c1[3] = {-40.0, 305.0, _goSafe};
float home[3] = {80, 260, -50};//;{0.0, 330.0, -100.0};

float pos_piez_inis[10][3] = {{-75, 279, _goSafe}, {-40, 279, _goSafe}, {0, 279, _goSafe}, {40, 279, _goSafe}, {73, 277, _goSafe}, {-75, 400, _goSafe}, {-40, 400, _goSafe}, {0, 400, _goSafe}, {40, 400, _goSafe}, {75, 400, _goSafe}};
int moving = 0;

/*################# ##
## SERIAL COMMMANDS ##
#####################*/
char serial_command_buffer_[32];
SerialCommands cmd(&Serial, serial_command_buffer_, sizeof(serial_command_buffer_), "\r\n", " ");

AlMar::Esp32::Driver_L298n* _m[N_MOTORS];
AlMar::Esp32::EncoderATM203_Spi2* _enc;
PID* _pid[N_MOTORS];

/*
TO DO:
- HELP
- SET_DEBUG [MODE]
- SET_MOTOR_DUTY [N] [DUTY]
- SET_MOTOR_POS [N] [POS]
- SET_MOTOR_KP [N] [KP]
- SET_MOTOR_KI [N] [KI]
- SET_MOTOR_KD [N] [KD]

- SET_MOTOR_ZERO [N]

- PRINT_MOTOR_INFO [N]

OPTIONAL:
- SET_MOTOR_DT [N] [DT]
- SET_MOTOR_MAX [N] [MAX]
- SET_MOTOR_MIN [N] [MIN]
*/

void goZ()
{
  des_tcp[2] = _goZ;
  Serial.printf("Go Z\n");
}

void goSafe()
{
  des_tcp[2] = _goSafe;
  Serial.printf("Go Safe\n");
}

void openGripper()
{
  motorPinza.write(0);
  Serial.printf("Abrir pinza\n");
}

void closeGripper()
{
  motorPinza.write(30);
  Serial.printf("Cerrar pinza\n");
}

void goHome()
{
  des_tcp[0] = home[0];
  des_tcp[1] = home[1];
  des_tcp[2] = home[2];
  Serial.printf("Go Home\n");
}

void goTCP(float x, float y, float z)
{
  des_tcp[0] = x;
  des_tcp[1] = y;
  des_tcp[2] = z;
  //Serial.printf("goTCP # x:%f, y:%f, z:%f\n", x, y, z);
}

int moverPieza(int pos_pieza, int pos_tab_obj) {
    float pos_final_obj[3] = {0, 0, 0};

    if(pos_pieza == -1 || pos_tab_obj == -1)
    {
      //Serial.println("ERROR: No se ha especificado la ficha o la posición");
      return 0;
    }

    
    
    switch (pos_tab_obj) {
        case 1:
            pos_final_obj[0] = c1[0];
            pos_final_obj[1] = c1[1];
            pos_final_obj[2] = c1[2];
        break;
        case 2:
            pos_final_obj[0] = c2[0];
            pos_final_obj[1] = c2[1];
            pos_final_obj[2] = c2[2];
        break;
        case 3:
            pos_final_obj[0] = c3[0];
            pos_final_obj[1] = c3[1];
            pos_final_obj[2] = c3[2];
        break;
        case 4:
            pos_final_obj[0] = c4[0];
            pos_final_obj[1] = c4[1];
            pos_final_obj[2] = c4[2];
        break;
        case 5: 
            pos_final_obj[0] = c5[0];
            pos_final_obj[1] = c5[1];
            pos_final_obj[2] = c5[2];
        break;
        case 6:
            pos_final_obj[0] = c6[0];
            pos_final_obj[1] = c6[1];
            pos_final_obj[2] = c6[2];
        break;
        case 7:
            pos_final_obj[0] = c7[0];
            pos_final_obj[1] = c7[1];
            pos_final_obj[2] = c7[2];
        break;
        case 8:
            pos_final_obj[0] = c8[0];
            pos_final_obj[1] = c8[1];
            pos_final_obj[2] = c8[2];
        break;
        case 9: 
            pos_final_obj[0] = c9[0];
            pos_final_obj[1] = c9[1];
            pos_final_obj[2] = c9[2];
        break;
    }
    /*Serial.printf("Mover ficha %i a posición %i\n", pos_pieza, pos_tab_obj);
    //Serial.printf("pos_final_obj: %f, %f, %f\n", pos_final_obj[0], pos_final_obj[1], pos_final_obj[2]);
    Serial.printf("end_ctrl: %i\n", end_ctrl);
    Serial.printf("estado: %i\n", estado);*/

    // Se supone que se parte siempre de la posición de reposo del robot
    if(end_ctrl == 1 && estado == 0) {
      //Serial.printf("Movimiento uno\n");
      goTCP(pos_piez_inis[pos_pieza-1][0], pos_piez_inis[pos_pieza-1][1], pos_piez_inis[pos_pieza-1][2]); // Se aproxima a la pieza inicial
      if(counter == 15)
      {
        estado = 1;
      }
      end_ctrl = 0;
    }

    if(end_ctrl == 1 && estado == 1) {
      //Serial.printf("Movimiento dos\n");
      goZ(); // Mantener esas coordenadas y bajar solo en z
      if(counter == 15)
      {
        estado = 2;
      }
      end_ctrl = 0;
    }

    if(end_ctrl == 1 && estado == 2) {
      //Serial.printf("Movimiento tres\n");
      closeGripper(); // Cerrar la pinza
      
      if(counter == 15)
      {
        estado = 3;
      }
      end_ctrl = 0;
    }

    if(end_ctrl == 1 && estado == 3) {
      goSafe(); // Mantener esas coordenadas y subir solo en z (ya con la pieza)
      if(counter == 15)
      {
        estado = 4;
      }
      end_ctrl = 0;
    }

    if(end_ctrl == 1 && estado == 4) {
      goTCP(pos_final_obj[0], pos_final_obj[1], pos_final_obj[2]); // Se aproxima a la posición objetivo
      if(counter == 15)
      {
        estado = 5;
      }
      end_ctrl = 0;
    }

    if(end_ctrl == 1 && estado == 5) {
      goZ(); // Mantener esas coordenadas y bajar solo en z
      if(counter == 15)
      {
        estado = 6;
      }
      end_ctrl = 0;
    }

    if(end_ctrl == 1 && estado == 6) {
      openGripper(); // Abrir la pinza
      if(counter == 15)
      {
        estado = 7;
      }
      end_ctrl = 0;
    }

    if(end_ctrl == 1 && estado == 7) {
      goSafe(); // Mantener esas coordenadas y subir solo en z (sin la pieza)
      if(counter == 15)
      {
        estado = 8;
      }
      end_ctrl = 0;
    }

    if(end_ctrl == 1 && estado == 8) {
      goTCP(home[0], home[1], home[2]); // Se lleva el robot a la posición de reposo
      if(counter == 15)
      {
        estado = 9;
      }
      end_ctrl = 0;
    }
    return 1; // Devuelve 1 para indicar la finalización del movimiento
}

void cmd_moverPieza(SerialCommands* sender)
{
  char* pos_pieza_str = sender->Next();
  char* pos_tab_obj_str = sender->Next();

  if(pos_pieza_str == NULL || pos_tab_obj_str == NULL)
  {
    Serial.println("ERROR NO_ARGS");
    return;
  }

  int pos_pieza = atoi(pos_pieza_str);
  int pos_tab_obj = atoi(pos_tab_obj_str);

  if(pos_pieza < 1 || pos_pieza > 10)
  {
    Serial.printf("Posición de pieza fuera de rango: %i\n", pos_pieza);
    return;
  }

  if(pos_tab_obj < 1 || pos_tab_obj > 9)
  {
    Serial.printf("Posición de tablero fuera de rango: %i\n", pos_tab_obj);
    return;
  }
  estado = 0;
  //moverPieza(pos_pieza, pos_tab_obj);
  ficha = pos_pieza;
  posicion = pos_tab_obj;
}

void cmd_goHome(SerialCommands* sender)
{
  goHome();
}

void cmd_goZ(SerialCommands* sender)
{
  goZ();
}

void cmd_goSafe(SerialCommands* sender)
{
  goSafe();
}

void cmd_openGripper(SerialCommands* sender)
{
  openGripper();
}

void cmd_closeGripper(SerialCommands* sender)
{
  closeGripper();
}

//This is the default handler, and gets called when no other command matches. 
void cmd_unrecognized(SerialCommands* sender, const char* cmd)
{
  sender->GetSerial()->print("El comando [");
  sender->GetSerial()->print(cmd);
  sender->GetSerial()->println("] no existe");
}


void set_debug(SerialCommands* sender)
{
  char* mode_str = sender->Next();
	if (mode_str == NULL)
	{
		Serial.println("ERROR NO_MODE");
		return;
	}
	int mode = atoi(mode_str);
  
  DEBUG = mode;
  Serial.printf("Debug mode set to %i\n", mode);
}

void cmd_set_led1(SerialCommands* sender)
{
  char* pwm_str = sender->Next();
	if (pwm_str == NULL)
	{
		Serial.println("ERROR NO_PWM");
		return;
	}

	int pwm = atoi(pwm_str);
  if(pwm < 0 || pwm > 255)
  {
    Serial.printf("PWM value out of range: %i\n", pwm);
    return;
  }
  
  if(DEBUG & DEBUG_INFO != 0)
  {
    Serial.printf("Led 1 set to %i\n", pwm);
  }

  analogWrite(led_1, pwm);
}

void cmd_set_led2(SerialCommands* sender)
{
  char* pwm_str = sender->Next();
	if (pwm_str == NULL)
	{
		Serial.println("ERROR NO_PWM");
		return;
	}

	int pwm = atoi(pwm_str);
  if(pwm < 0 || pwm > 255)
  {
    Serial.printf("PWM value out of range: %i\n", pwm);
    return;
  }
  
  if(DEBUG & DEBUG_INFO != 0)
  {
    Serial.printf("Led 2 set to %i\n", pwm);
  }

  analogWrite(led_2, pwm);
}

void cmd_set_motor_pos(SerialCommands* sender)
{
  char* n_str = sender->Next();
  char* pos_str = sender->Next();

  if(n_str == NULL || pos_str == NULL)
  {
    Serial.println("ERROR NO_ARGS");
    return;
  }

  int n = atoi(n_str);
  float dPos = atof(pos_str);

  if(n < 0 || n >= N_MOTORS)
  {
    Serial.printf("Motor number out of range: %i\n", n);
    return;
  }

  if(dPos < -360 || dPos > 360)
  {
    Serial.printf("Position value out of range: %f\n", dPos);
    return;
  }

  desPos[n] = dPos;
  _pid[n]->reset();

  if(DEBUG & DEBUG_INFO != 0)
  {
    Serial.printf("Motor %i set to %f\n", n, dPos);
  }

  /*TCP_d = arrayTLineal(pos[0], 250, -140, desPos[0], 250, -140);
  for(int i = 0; i < 50; i++)
  {
    Serial.printf("TCP_d[%i] >> base:%f, codo:%f, hombro:%f\n", i, i, TCP_d[i][0], TCP_d[i][1], TCP_d[i][2]);
  }*/

}

void cmd_set_encoder_zero(SerialCommands* sender)
{
  char* n_str = sender->Next();
  
  if(n_str == NULL)
  {
    Serial.println("ERROR NO_ARGS");
    return;
  }

  int n = atoi(n_str);
  _enc->SetZero(n);

  if(DEBUG & DEBUG_INFO != 0)
  {
    Serial.printf("Encoder %i set to zero\n", n);
  }
}

void cmd_set_motor_kp(SerialCommands* sender)
{
  char* n_str = sender->Next();
  char* kp_str = sender->Next();

  if(n_str == NULL || kp_str == NULL)
  {
    Serial.println("ERROR NO_ARGS");
    return;
  }

  int n = atoi(n_str);
  float kp = atof(kp_str);

  if(n < 0 || n >= N_MOTORS)
  {
    Serial.printf("Motor number out of range: %i\n", n);
    return;
  }

  _pid[n]->setKP(kp);

  if(DEBUG & DEBUG_INFO != 0)
  {
    Serial.printf("Motor %i Kp set to %f\n", n, kp);
  }
}

void cmd_set_motor_ki(SerialCommands* sender)
{
  char* n_str = sender->Next();
  char* ki_str = sender->Next();

  if(n_str == NULL || ki_str == NULL)
  {
    Serial.println("ERROR NO_ARGS");
    return;
  }

  int n = atoi(n_str);
  float ki = atof(ki_str);

  if(n < 0 || n >= N_MOTORS)
  {
    Serial.printf("Motor number out of range: %i\n", n);
    return;
  }

  _pid[n]->setKI(ki);

  if(DEBUG & DEBUG_INFO != 0)
  {
    Serial.printf("Motor %i Ki set to %f\n", n, ki);
  }
}

void cmd_set_motor_kd(SerialCommands* sender)
{
  char* n_str = sender->Next();
  char* kd_str = sender->Next();

  if(n_str == NULL || kd_str == NULL)
  {
    Serial.println("ERROR NO_ARGS");
    return;
  }

  int n = atoi(n_str);
  float kd = atof(kd_str);

  if(n < 0 || n >= N_MOTORS)
  {
    Serial.printf("Motor number out of range: %i\n", n);
    return;
  }

  _pid[n]->setKD(kd);

  if(DEBUG & DEBUG_INFO != 0)
  {
    Serial.printf("Motor %i Kd set to %f\n", n, kd);
  }
}

void cmd_get_encoder_deg(SerialCommands* sender)
{
  char* n_str = sender->Next();
  
  if(n_str == NULL)
  {
    Serial.println("ERROR NO_ARGS");
    return;
  }

  int n = atoi(n_str);
  int pos = _enc->Read(n)*(360.0/4096.0);

  if(DEBUG & DEBUG_INFO != 0)
  {
    Serial.printf("Encoder %i position: %i\n", n, pos);
  }
}

void cmd_get_encoders_deg(SerialCommands* sender)
{
  for(int n = 0; n < N_MOTORS; n++)
  {
    int pos = _enc->Read(n)*(360.0/4096.0);
    Serial.printf("Encoder %i position: %f\n", n, pos);
  }
}

void cmd_get_motor_info(SerialCommands* sender)
{
  char* n_str = sender->Next();
  
  if(n_str == NULL)
  {
    Serial.println("ERROR NO_ARGS");
    return;
  }

  int n = atoi(n_str);
  _pid[n]->displayInfo();
}

void cmd_set_tcp(SerialCommands* sender)
{
  char* x_str = sender->Next();
  char* y_str = sender->Next();
  char* z_str = sender->Next();
  
  if(x_str == NULL || y_str == NULL || z_str == NULL)
  {
    Serial.println("ERROR NO_ARGS");
    return;
  }

  float x = atof(x_str);
  float y = atof(y_str);
  float z = atof(z_str);

  if(DEBUG & DEBUG_INFO != 0)
  {
    Serial.printf("Setting position to x:%f, y:%f, z:%f\n", x, y, z);
  }
  goTCP(x, y, z);
}

void cmd_get_tcp(SerialCommands* sender)
{
  Serial.printf("TCP position: x:%f, y:%f, z:%f\n", tcp[0], tcp[1], tcp[2]);
}

void cmd_goto(SerialCommands* sender)
{
  char* n_str = sender->Next();
  
  if(n_str == NULL)
  {
    Serial.println("ERROR NO_ARGS");
    return;
  }

  int n = atoi(n_str);

  switch(n) 
    {
      case 1:
        des_tcp[0] = c1[0];
        des_tcp[1] = c1[1];
        des_tcp[2] = c1[2];
        break;
      case 2:
        des_tcp[0] = c2[0];
        des_tcp[1] = c2[1];
        des_tcp[2] = c2[2];
        break;
      case 3:
        des_tcp[0] = c3[0];
        des_tcp[1] = c3[1];
        des_tcp[2] = c3[2];
        break;
      case 4:
        des_tcp[0] = c4[0];
        des_tcp[1] = c4[1];
        des_tcp[2] = c4[2];
        break;
      case 5:
        des_tcp[0] = c5[0];
        des_tcp[1] = c5[1];
        des_tcp[2] = c5[2];
        break;
      case 6:
        des_tcp[0] = c6[0];
        des_tcp[1] = c6[1];
        des_tcp[2] = c6[2];
        break;
      case 7:
        des_tcp[0] = c7[0];
        des_tcp[1] = c7[1];
        des_tcp[2] = c7[2];
        break;
      case 8:
        des_tcp[0] = c8[0];
        des_tcp[1] = c8[1];
        des_tcp[2] = c8[2];
        break;
      case 9:
        des_tcp[0] = c9[0];
        des_tcp[1] = c9[1];
        des_tcp[2] = c9[2];
        break;
      case 15:
        des_tcp[0] = home[0];
        des_tcp[1] = home[1];
        des_tcp[2] = home[2];
        break;
      case 16:
        des_tcp[2] = _goZ;
        break;
      case 17:
        des_tcp[2] = _goSafe;
        break;
      case 20:
        des_tcp[0] = pos_piez_inis[0][0];
        des_tcp[1] = pos_piez_inis[0][1];
        des_tcp[2] = pos_piez_inis[0][2];
        break;
      case 21:
        des_tcp[0] = pos_piez_inis[1][0];
        des_tcp[1] = pos_piez_inis[1][1];
        des_tcp[2] = pos_piez_inis[1][2];
        break;
      case 22:
        des_tcp[0] = pos_piez_inis[2][0];
        des_tcp[1] = pos_piez_inis[2][1];
        des_tcp[2] = pos_piez_inis[2][2];
        break;
      case 23:
        des_tcp[0] = pos_piez_inis[3][0];
        des_tcp[1] = pos_piez_inis[3][1];
        des_tcp[2] = pos_piez_inis[3][2];
        break;
      case 24:
        des_tcp[0] = pos_piez_inis[4][0];
        des_tcp[1] = pos_piez_inis[4][1];
        des_tcp[2] = pos_piez_inis[4][2];
        break;
      case 25:
        des_tcp[0] = pos_piez_inis[5][0];
        des_tcp[1] = pos_piez_inis[5][1];
        des_tcp[2] = pos_piez_inis[5][2];
        break;
      case 26:
        des_tcp[0] = pos_piez_inis[6][0];
        des_tcp[1] = pos_piez_inis[6][1];
        des_tcp[2] = pos_piez_inis[6][2];
        break;
      case 27:
        des_tcp[0] = pos_piez_inis[7][0];
        des_tcp[1] = pos_piez_inis[7][1];
        des_tcp[2] = pos_piez_inis[7][2];
        break;
      case 28:
        des_tcp[0] = pos_piez_inis[8][0];
        des_tcp[1] = pos_piez_inis[8][1];
        des_tcp[2] = pos_piez_inis[8][2];
        break;
      case 29:
        des_tcp[0] = pos_piez_inis[9][0];
        des_tcp[1] = pos_piez_inis[9][1];
        des_tcp[2] = pos_piez_inis[9][2];
        break;
      default:
        Serial.printf("Position number out of range: %i\n", n);
        return;

    }
}

SerialCommand cmd_set_debug_("SET_DEBUG", set_debug);

SerialCommand cmd_set_led1_("SET_LED1", cmd_set_led1);
SerialCommand cmd_set_led2_("SET_LED2", cmd_set_led2);

SerialCommand cmd_set_motor_pos_("SET_MOTOR_POS", cmd_set_motor_pos);
SerialCommand cmd_set_encoder_zero_("SET_ENCODER_ZERO", cmd_set_encoder_zero);

SerialCommand cmd_set_motor_kp_("SET_MOTOR_KP", cmd_set_motor_kp);
SerialCommand cmd_set_motor_ki_("SET_MOTOR_KI", cmd_set_motor_ki);
SerialCommand cmd_set_motor_kd_("SET_MOTOR_KD", cmd_set_motor_kd);

SerialCommand cmd_get_encoder_deg_("GET_ENCODER_DEG", cmd_get_encoder_deg);
SerialCommand cmd_get_encoders_deg_("GET_ENCODERS_DEG", cmd_get_encoders_deg);
SerialCommand cmd_get_motor_info_("GET_MOTOR_INFO", cmd_get_motor_info);

SerialCommand cmd_set_tcp_("SET_TCP", cmd_set_tcp);
SerialCommand cmd_get_tcp_("GET_TCP", cmd_get_tcp);

SerialCommand cmd_goto_("GOTO", cmd_goto);

SerialCommand cmd_goZ_("GO_Z", cmd_goZ);
SerialCommand cmd_goSafe_("GO_SAFE", cmd_goSafe);
SerialCommand cmd_goHome_("GO_HOME", cmd_goHome);
SerialCommand cmd_openGripper_("OPEN_GRIPPER", cmd_openGripper);
SerialCommand cmd_closeGripper_("CLOSE_GRIPPER", cmd_closeGripper);

SerialCommand cmd_moverPieza_("MOVER_PIEZA", cmd_moverPieza);