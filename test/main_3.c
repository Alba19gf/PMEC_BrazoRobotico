#include <Arduino.h>
#include <SPI.h>

// SPI PINS
#define SPI_SS    10
#define SPI_MOSI  11
#define SPI_CLK   12
#define SPI_MISO  13


// ENCODER AMT203-V SPI COMMANDS 
#define NOP_A5          0x00        // No operation
#define RD_POS          0x10        // Read position
#define SET_ZERO_POINT  0x70        // Zero set

#define timoutLimit 100

const int motor1Pin1 = 17;
const int motor1Pin2 = 15;

const int ledChannel = 0;
const int frequency = 50000;
const int resolution = 8;
int dutyCycle = 125;

String cmd;

uint8_t SPIWrite(uint8_t sendByte)
{
  //holder for the received over SPI
  uint8_t data;

  //the AMT20 requires the release of the CS line after each byte
  digitalWrite(SPI_SS, LOW);
  data = SPI.transfer(sendByte);
  digitalWrite(SPI_SS, HIGH);

  //we will delay here to prevent the AMT20 from having to prioritize SPI over obtaining our position
  delayMicroseconds(10);
  
  return data;
}

void setup() {

  // Pin motors
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  // PWM signal
  ledcSetup(ledChannel, frequency, resolution);
  //ledcAttachPin(motor1Pin1, ledChannel);
  ledcAttachPin(motor1Pin2, ledChannel);

  // SPI INIT
  pinMode(SPI_CLK, OUTPUT);
  pinMode(SPI_MOSI, OUTPUT);
  pinMode(SPI_MISO, INPUT);
  pinMode(SPI_SS, OUTPUT);

  SPI.beginTransaction(SPISettings(500000, MSBFIRST, SPI_MODE0));

  digitalWrite(SPI_SS, HIGH);
  //SPI.begin(SPI_CLK, SPI_MISO, SPI_MOSI, SPI_SS);

  //SPI CONFIG
  /*SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider(SPI_CLOCK_DIV32);
  */
  Serial.begin(115200);
  Serial.print("Testing DC Motor...");
}

void loop() {

  // CMD
  while(Serial.available()) {

    cmd = Serial.readString();// read the incoming data as string
    cmd.trim();

    Serial.printf("\nCMD: %s\n", cmd);
    
    if(cmd.equals("H"))
    {
      digitalWrite(SPI_SS, HIGH);
      Serial.println("SS HIGH");
    }

    else if(cmd.equals("L"))
    {
      digitalWrite(SPI_SS, LOW);
      Serial.println("SS LOW");
    }

    else if(cmd.toInt() >= 0 && cmd.toInt() <= 255)
    {
      dutyCycle = cmd.toInt();
      Serial.printf("New duty cycle: %i \n", dutyCycle);
    }
  }
  
  digitalWrite(motor1Pin1, LOW);
  ledcWrite(ledChannel, dutyCycle);

}