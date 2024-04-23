#include <Arduino.h>
#include <SPI.h>
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
}