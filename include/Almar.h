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
                      {45, 7, 6, 25000, 0.45, 0.1, 0.00, 360, -360, 0.425},      // Motor 2
                      {39, 10, 9, 25000, 0.4, 0.1, 0.00, 360, -360, 0.6}      // Motor 3
                    };
// Cantidad motores
const int N_MOTORS = sizeof(motor)/sizeof(motor[0]);

// Encoder
int cs_pins[]={PIN_CS_M1, PIN_CS_M2, PIN_CS_M3};
int pwm_freq = 25000;

// Kinematics
float** TCP_d;

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
float _goSafe = -100;

float c9[3] = {39.0,  369.0, -100.0};
float c8[3] = { 0.0,  369.0, -100.0};
float c7[3] = {-39.0, 369.0, -100.0};
float c6[3] = {39.0,  330.0, -100.0};
float c5[3] = { 0.0,  330.0, -100.0};
float c4[3] = {-39.0, 330.0, -100.0};
float c3[3] = {39.0,  290.0, -100.0};
float c2[3] = { 0.0,  290.0, -100.0};
float c1[3] = {-39.0, 290.0, -100.0};
float home[3] = {c1[0], c1[1], c1[2]};//;{0.0, 330.0, -100.0};


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
  motorPinza.write(28);
  Serial.printf("Cerrar pinza\n");
}

void goTCP(float x, float y, float z)
{
  des_tcp[0] = x;
  des_tcp[1] = y;
  des_tcp[2] = z;
  Serial.printf("goTCP # x:%f, y:%f, z:%f\n", x, y, z);
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

  if(n < 0 || n > 9)
  {
    Serial.printf("Position number out of range: %i\n", n);
    return;
  }

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
SerialCommand cmd_openGripper_("OPEN_GRIPPER", cmd_openGripper);
SerialCommand cmd_closeGripper_("CLOSE_GRIPPER", cmd_closeGripper);