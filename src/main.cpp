#include <Arduino.h>
#include <SPI.h>
<<<<<<< HEAD
#include "../include/Almar.h"

hw_timer_t *timer = NULL;

int ctrl = false;
void IRAM_ATTR timerCtrl() {
 ctrl = true;
}

void setup() {
  // Inicializamos el puerto serial
  Serial.begin(115200);
  // Inicializamos el puerto serial para los comandos
  // Comando por defecto para comandos no reconocidos
  cmd.SetDefaultHandler(cmd_unrecognized);
  // Comandos
  cmd.AddCommand(&cmd_set_debug_);
  cmd.AddCommand(&cmd_set_led1_);
  cmd.AddCommand(&cmd_set_led2_);
  cmd.AddCommand(&cmd_set_motor_pos_);
  cmd.AddCommand(&cmd_set_encoder_zero_);
  cmd.AddCommand(&cmd_set_motor_kp_);
  cmd.AddCommand(&cmd_set_motor_ki_);
  cmd.AddCommand(&cmd_set_motor_kd_);

  // Inicializamos el timer
  timer = timerBegin(0, 80, true);                // Timer 0, clock divider 80
  timerAttachInterrupt(timer, &timerCtrl, true);  // Attach the interrupt handling function
  timerAlarmWrite(timer, 1000000, true);          // Interrupt every 1 second
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
  }
  // Inicializamos los encoders
  _enc = new AlMar::Esp32::EncoderATM203_Spi2(cs_pins,N_MOTORS,PIN_MOSI,PIN_MISO,PIN_SCLK);
}

void loop() {
  cmd.ReadSerial();

  if(ctrl)
  {
     // Tasks to perform when the Timer interrupt is triggered
    //digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); // Toggle the state of the LED
    int pos=_enc->Read(0); // lee encoder 0 (M1)
    int pos2=_enc->Read(1); // lee encoder 1 (M2)
    int pos3=_enc->Read(2); // lee encoder 2 (M3)

    if(pos!=0x80000000) { 
      posOld = abs(posD);
      posD = (float) (pos*360.0/4096.0);
      
      dutyCycle = _pid[0]->calc(posD, desPos);
      
      _m[0]->SetDuty(dutyCycle);
    }


    ctrl = false;
  }
=======
//#include "../lib/esp_random.h"
#include "../lib/ALMar_ESP32_Driver_L298n.cpp"
#include "../lib/ALMar_ESP32_EncoderATM203_Spi2.cpp"
#include "../include/PID.cpp"

#define PIN_CS_M1  10 // (chip select para encoder de motor 1) 10 for SPI3
#define PIN_CS_M2  40  // (chip select para encoder de motor 2) 
#define PIN_CS_M3  41  // (chip select para encoder de motor 3) 
// Pines comunes a todos los encóders:
#define PIN_SCLK  12 // 36 // 12 for SPI3
#define PIN_MISO  13 // 37 // 13 for SPI3
#define PIN_MOSI  11 // 35 // 11 for SPI3

#define N_MOTORS  3 // 35 // 11 for SPI3

// Motores
int en1_pin = 17;
int motor1_pin1 = 16;
int motor1_pin2 = 15;

// Encoder
int cs_pins[]={PIN_CS_M1,PIN_CS_M2,PIN_CS_M3};

int pwm_freq = 20000;
float duty = -1;

// PID
float dutyCycle = 0;
float desPos = 100.0;
float posD = 0.0;

float Kp = 0.02;
float Ki = 0.002;
float Kd = 0.001;

float dt = 0.1;
float max_u = 1;
float min_u = -1;

AlMar::Esp32::Driver_L298n* _m1; // Se puede usar un array en su lugar: *_motors[3];  
AlMar::Esp32::EncoderATM203_Spi2* _enc;
PID* _pid;

String cmd;

void setup() {
  Serial.begin(9600);
  
  _m1 = new AlMar::Esp32::Driver_L298n(en1_pin, motor1_pin1, motor1_pin2, pwm_freq);
  _m1->begin();

  _enc = new AlMar::Esp32::EncoderATM203_Spi2(cs_pins,N_MOTORS,PIN_MOSI,PIN_MISO,PIN_SCLK);
  _enc->SetZero(0);

  _pid = new PID(Kp, Ki, Kd, dt, max_u, min_u);
}

void loop() {

    // CMD
  while(Serial.available()) 
  {
    cmd = Serial.readString();// read the incoming data as string
    cmd.trim();

    Serial.printf("\nCMD: %s\n", cmd);

    if(cmd.toFloat() >= -1 && cmd.toFloat() <= 1)
    {
      dutyCycle = cmd.toFloat();
      _m1->SetDuty(dutyCycle); 
      Serial.printf("New duty cycle: %f \n", dutyCycle);
      delay(1000);
    }
    else
    {
      desPos = cmd.toFloat();
      Serial.printf("New desired position: %f \n", desPos);
    }
    
  }

  int pos=_enc->Read(0); // lee encoder 0 (M1)

  if(pos!=0x80000000) { 
    posD = (float) (_enc->Read(0)*360.0/4096.0);
    dutyCycle = _pid->calc(posD, desPos);
    _m1->SetDuty(dutyCycle);
  }

  delay(100);
>>>>>>> 0131d2e687cf24149704c2174ceff6295174a4da
}